// Fill out your copyright notice in the Description page of Project Settings.


#include "RightPortal.h"

// Sets default values
ARightPortal::ARightPortal()
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
		static ConstructorHelpers::FObjectFinder<UMaterial>PortalMaterial(TEXT("Material'/Game/Materials/LeftPortalRT_Mat.LeftPortalRT_Mat'")); //to display left view on right
		if (PortalMaterial.Succeeded())
		{
			Plane->SetMaterial(0, PortalMaterial.Object);
		}

	}

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture"));
	SceneCapture->SetupAttachment(PortalRootComponent);
	//SceneCapture->SetupAttachment(Plane);
	SceneCapture->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>RenderTarget(TEXT("TextureRenderTarget2D'/Game/Textures/RightPortalRT.RightPortalRT'"));
	if (RenderTarget.Succeeded())
	{
		SceneCapture->TextureTarget = RenderTarget.Object;
	}

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(PortalRootComponent);
	//Box->SetupAttachment(Plane);
	Box->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	Box->SetWorldScale3D(FVector(0.25f, 1.5f, 3.25f));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(PortalRootComponent);
	//Arrow->SetupAttachment(Plane);
	Arrow->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

	CharacterRef = Cast<AGateSDKCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called when the game starts or when spawned
void ARightPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARightPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetRenderTargetRotation();
}

void ARightPortal::SetplayerVelocity()
{
}

void ARightPortal::SetPlayerRotation()
{
}

void ARightPortal::ResetMaxVelocity()
{
}

void ARightPortal::SetVelocityAccordingToForwardVector()
{
}

void ARightPortal::SetPortalLocationOnCall()
{
}

void ARightPortal::SetRenderTargetRotation()
{
	PlayerRotation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraRotation();
	//LeftPortalForwardVector = CharacterRef->GetActorForwardVector();
	LeftPortalForwardVector = CharacterRef->GetLeftPortalForwardVector();
	if (LeftPortalForwardVector.X == 1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, ((PlayerRotation.Yaw - 180.f) * -1), 0.f));
	}
	else if (LeftPortalForwardVector.X == -1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, PlayerRotation.Yaw, 0.f));
	}
	else if (LeftPortalForwardVector.Y == 1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, ((PlayerRotation.Yaw - -90.f) * -1), 0.f));
	}
	else if (LeftPortalForwardVector.Y == -1)
	{
		SceneCapture->SetRelativeRotation(FRotator(0.f, ((PlayerRotation.Yaw - 90.f) * -1), 0.f));
	}
}