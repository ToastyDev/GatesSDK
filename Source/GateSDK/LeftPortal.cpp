// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftPortal.h"

// Sets default values
ALeftPortal::ALeftPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Plane->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneAsset(TEXT(""));
	if (PlaneAsset.Succeeded())
	{
		Plane->SetStaticMesh(PlaneAsset.Object);
		Plane->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Plane->SetRelativeRotation();
		Plane->SetWorldScale3D(FVector(0.f));
	}

	SceneCapture->SetupAttachment(RootComponent);

	Box->SetupAttachment(RootComponent);
	Box->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

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

}

void ALeftPortal::SetplayerVelocity()
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

void ALeftPortal::SetRenderTargetLocation()
{
}