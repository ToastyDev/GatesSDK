// Copyright Epic Games, Inc. All Rights Reserved.

#include "GateSDKCharacter.h"
#include "GateSDKProjectile.h"
#include "LeftPortal.h"
#include "RightPortal.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"

#include "DrawDebugHelpers.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGateSDKCharacter

AGateSDKCharacter::AGateSDKCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	//TriggerComponent = GetCapsuleComponent();
	//TriggerComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	//TriggerComponent->SetupAttachment(GetCapsuleComponent());
	//TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AGateSDKCharacter::OnBeginActorOverlap);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void AGateSDKCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	
	Mesh1P->SetHiddenInGame(false, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGateSDKCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Portal Actions
	PlayerInputComponent->BindAction("Spawn Left Portal", IE_Pressed, this, &AGateSDKCharacter::SpawnLeftPortal);
	PlayerInputComponent->BindAction("Destroy Left Portal", IE_Pressed, this, &AGateSDKCharacter::DestroyLeftPortal);
	PlayerInputComponent->BindAction("Spawn Right Portal", IE_Pressed, this, &AGateSDKCharacter::SpawnRightPortal);
	PlayerInputComponent->BindAction("Destroy Right Portal", IE_Pressed, this, &AGateSDKCharacter::DestroyRightPortal);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGateSDKCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AGateSDKCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGateSDKCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGateSDKCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGateSDKCharacter::LookUpAtRate);
}

void AGateSDKCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AGateSDKProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AGateSDKCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGateSDKCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGateSDKCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGateSDKCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGateSDKCharacter::SpawnLeftPortal()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Left Portal Pressed"));

	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FRotator CameraRotation = FirstPersonCameraComponent->GetComponentRotation();

	//FVector TraceEndPoint = CameraLocation + (CameraRotation.Vector() * 10000.f);
	FVector TraceEndPoint = CameraLocation + (CameraRotation.Vector() * 10000.f);

	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult HitDetails = FHitResult(ForceInit);

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitDetails, CameraLocation, TraceEndPoint, ECC_GameTraceChannel3, TraceParams);
	if (bHasHit)
	{
		DrawDebugLine(GetWorld(), CameraLocation, HitDetails.ImpactPoint, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
		DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Left Portal Hit"));

		if (LeftPortalClass != NULL /* && !bLeftPortalSpawned*/)
		{
			if (bLeftPortalSpawned && StoredLeftPortal != NULL)
				DestroyLeftPortal();

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//push off surface so not inside
			//TODO: Define 45 degree
			if (HitDetails.ImpactNormal.X == 1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(1.f, 0.f, 0.f);
				StoredLeftPortal = GetWorld()->SpawnActor<ALeftPortal>(LeftPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.X == -1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(-1.f, 0.f, 0.f);
				StoredLeftPortal = GetWorld()->SpawnActor<ALeftPortal>(LeftPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Y == 1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, 1.f, 0.f);
				StoredLeftPortal = GetWorld()->SpawnActor<ALeftPortal>(LeftPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Y == -1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, -1.f, 0.f);
				StoredLeftPortal = GetWorld()->SpawnActor<ALeftPortal>(LeftPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Z == 1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, 0.f, 1.f);
				StoredLeftPortal = GetWorld()->SpawnActor<ALeftPortal>(LeftPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Z == -1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, 0.f, -1.f);
				StoredLeftPortal = GetWorld()->SpawnActor<ALeftPortal>(LeftPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			
			bLeftPortalSpawned = true;
			//LeftPortalForwardVector = StoredLeftPortal->GetActorForwardVector();
			LeftPortalForwardVector = HitDetails.ImpactNormal;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), CameraLocation, TraceEndPoint, FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Left Portal Missed"));
	}

}

void AGateSDKCharacter::DestroyLeftPortal()
{
	if (bLeftPortalSpawned)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Left Portal Destroyed"));
		LeftPortalForwardVector = FVector(NULL);
		LeftPortalLocation = FVector(NULL);
		GetWorld()->DestroyActor(StoredLeftPortal);
		bLeftPortalSpawned = false;
	}
}

FVector AGateSDKCharacter::GetLeftPortalForwardVector()
{
	return LeftPortalForwardVector;
}

void AGateSDKCharacter::SpawnRightPortal()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Right Portal Pressed"));

	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FRotator CameraRotation = FirstPersonCameraComponent->GetComponentRotation();

	FVector TraceEndPoint = CameraLocation + (CameraRotation.Vector() * 10000.f);

	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult HitDetails = FHitResult(ForceInit);

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitDetails, CameraLocation, TraceEndPoint, ECC_GameTraceChannel3, TraceParams);
	if (bHasHit)
	{
		DrawDebugLine(GetWorld(), CameraLocation, HitDetails.ImpactPoint, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
		DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Orange, false, 5.f, ECC_WorldStatic, 1.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Right Portal Hit"));
		
		if (RightPortalClass != NULL /* && !bRightPortalSpawned*/)
		{
			if (bRightPortalSpawned && StoredRightPortal != NULL)
				DestroyRightPortal();

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//push off surface so not inside
			// TODO define 45 degree
			if (HitDetails.ImpactNormal.X == 1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(1.f, 0.f, 0.f);
				StoredRightPortal = GetWorld()->SpawnActor<ARightPortal>(RightPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.X == -1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(-1.f, 0.f, 0.f);
				StoredRightPortal = GetWorld()->SpawnActor<ARightPortal>(RightPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Y == 1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, 1.f, 0.f);
				StoredRightPortal = GetWorld()->SpawnActor<ARightPortal>(RightPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Y == -1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, -1.f, 0.f);
				StoredRightPortal = GetWorld()->SpawnActor<ARightPortal>(RightPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Z == 1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, 0.f, 1.f);
				StoredRightPortal = GetWorld()->SpawnActor<ARightPortal>(RightPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			else if (HitDetails.ImpactNormal.Z == -1)
			{
				FVector SpawnLoc = HitDetails.ImpactPoint + FVector(0.f, 0.f, -1.f);
				StoredRightPortal = GetWorld()->SpawnActor<ARightPortal>(RightPortalClass, SpawnLoc, HitDetails.ImpactNormal.Rotation(), ActorSpawnParams);
			}
			
			bRightPortalSpawned = true;
			//RightPortalForwardVector = StoredRightPortal->GetActorForwardVector();
			RightPortalForwardVector = HitDetails.ImpactNormal;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), CameraLocation, TraceEndPoint, FColor::Orange, false, 5.f, ECC_WorldStatic, 1.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Right Portal Missed"));
	}
}

void AGateSDKCharacter::DestroyRightPortal()
{
	if (bRightPortalSpawned)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Right Portal Destroyed"));
		RightPortalForwardVector = FVector(NULL);
		RightPortalLocation = FVector(NULL);
		GetWorld()->DestroyActor(StoredRightPortal);
		bRightPortalSpawned = false;
	}
}

FVector AGateSDKCharacter::GetRightPortalForwardVector()
{
	return RightPortalForwardVector;
}

void AGateSDKCharacter::StorePlayerVelocity()
{
	SavedPlayerVelocity = GetVelocity();
}

void AGateSDKCharacter::LeftPortalCollisionCheck()
{

}

void AGateSDKCharacter::RightPortalCollisionCheck()
{

}

bool AGateSDKCharacter::GetCanTeleport()
{
	return bCanTeleport;
}

void AGateSDKCharacter::SetCanTeleport(bool bCanTele)
{
	bCanTeleport = bCanTele;
}

FVector AGateSDKCharacter::GetRightPortalLocation()
{
	if (StoredRightPortal != NULL)
		return StoredRightPortal->GetActorLocation();
	else
		return FVector(NULL);
}

FVector AGateSDKCharacter::GetLeftPortalLocation()
{
	if (StoredLeftPortal != NULL)
		return StoredLeftPortal->GetActorLocation();
	else
		return FVector(NULL);
}

/*void AGateSDKCharacter::OnBeginActorOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	//left
	if (otherActor && (otherActor != this) && otherComponent && otherActor->GetClass()->IsChildOf(ALeftPortal::StaticClass()))
	{
		StorePlayerVelocity();
		SetActorLocation(StoredRightPortal->GetActorLocation());
	}
	//right
	if (otherActor && (otherActor != this) && otherComponent && otherActor->GetClass()->IsChildOf(ARightPortal::StaticClass()))
	{
		StorePlayerVelocity();
		SetActorLocation(StoredLeftPortal->GetActorLocation());
	}
}*/