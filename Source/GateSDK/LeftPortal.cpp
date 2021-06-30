// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftPortal.h"

// Sets default values
ALeftPortal::ALeftPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PortalRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(PortalRootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (PlaneAsset.Succeeded())
	{
		Plane->SetStaticMesh(PlaneAsset.Object);
		Plane->SetMobility(EComponentMobility::Movable);
		Plane->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Plane->SetRelativeRotation(FRotator(0.f, -90.f, 90.f));
		Plane->SetWorldScale3D(FVector(1.25f, 2.25f, 0.f));
		Plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		static ConstructorHelpers::FObjectFinder<UMaterial>PortalMaterial(TEXT("Material'/Game/Materials/RightPortalRT_Mat1.RightPortalRT_Mat1'")); //to display right view on left
		if (PortalMaterial.Succeeded())
		{
			RightPortalMat = PortalMaterial.Object;
			Plane->SetMaterial(0, PortalMaterial.Object);
		}	
	}

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture"));
	SceneCapture->SetupAttachment(PortalRootComponent);
	//SceneCapture->SetupAttachment(Plane);
	SceneCapture->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>RenderTarget(TEXT("TextureRenderTarget2D'/Game/Textures/LeftPortalRT.LeftPortalRT'"));
	if (RenderTarget.Succeeded())
	{
		SceneCapture->TextureTarget = RenderTarget.Object;
	}
		
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(PortalRootComponent);
	//Box->SetupAttachment(Plane);
	Box->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	Box->SetWorldScale3D(FVector(0.25f, 1.5f, 3.25f));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ALeftPortal::OnComponentBeginOverlap);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(PortalRootComponent);
	//Arrow->SetupAttachment(Plane);
	Arrow->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

	CharacterRef = Cast<AGateSDKCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	static ConstructorHelpers::FObjectFinder<UMaterial>RightPortalNotSpawnMat(TEXT("Material'/Game/Materials/NoRightPortalRTMat.NoRightPortalRTMat'"));
	if (RightPortalNotSpawnMat.Succeeded())
		RightPortalNotSpawnedMat = RightPortalNotSpawnMat.Object;
}

// Called when the game starts or when spawned
void ALeftPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeftPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharacterRef->StoredLeftPortal->SetActorLocation(GetActorLocation());

	if (CharacterRef->GetRightPortalLocation() != FVector(NULL))
	{
		Plane->SetMaterial(0, RightPortalMat);
		IsSecondPortalSpawned = true;
	}
	else
	{
		Plane->SetMaterial(0, RightPortalNotSpawnedMat);
		IsSecondPortalSpawned = false;
	}
	
	SetRenderTargetRotation();
}

void ALeftPortal::SetPlayerVelocity()
{

}

void ALeftPortal::SetPlayerRotation()
{
}

void ALeftPortal::ResetMaxVelocity()
{
}

void ALeftPortal::SetVelocityAccordingToForwardVector()
{
}

void ALeftPortal::SetPortalLocationOnCall()
{
}

void ALeftPortal::SetRenderTargetRotation()
{
	PlayerRotation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraRotation();
	//RightPortalForwardVector = CharacterRef->GetActorForwardVector();
	RightPortalForwardVector = CharacterRef->GetRightPortalForwardVector();
	if (RightPortalForwardVector.X == 1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, ((PlayerRotation.Yaw - 180.f) * -1), 0.f));
	}
	else if (RightPortalForwardVector.X == -1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, PlayerRotation.Yaw, 0.f));
	}
	else if (RightPortalForwardVector.Y == 1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, ((PlayerRotation.Yaw - -90.f) * -1), 0.f));
	}
	else if (RightPortalForwardVector.Y == -1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, ((PlayerRotation.Yaw - 90.f) * -1), 0.f));
	}
}

void ALeftPortal::OnComponentBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (IsSecondPortalSpawned)
	{
		if (otherActor == CharacterRef)
		{
			IsPlayer = true;
			LeftPortalForwardVector = CharacterRef->GetLeftPortalForwardVector();
			if (CharacterRef->GetCanTeleport())
			{
				CharacterRef->SetCanTeleport(false);
				CharacterRef->SetActorLocation(CharacterRef->GetRightPortalLocation(), false, nullptr, ETeleportType::TeleportPhysics);
				if (IsPlayer)
				{
					SetPlayerRotation();
					SetPlayerVelocity();
					CharacterRef->SetCanTeleport(true);
				}
			}
		}
		else
		{
			IsPlayer = false;
			LeftPortalForwardVector = CharacterRef->GetLeftPortalForwardVector();
		}
	}
}