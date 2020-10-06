// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftPortal.h"

// Sets default values
ALeftPortal::ALeftPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Plane->SetupAttachment(RootComponent);
	SceneCapture->SetupAttachment(RootComponent);
	Box->SetupAttachment(RootComponent);
	Arrow->SetupAttachment(RootComponent);

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