// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "LeftPortal.generated.h"

UCLASS()
class GATESDK_API ALeftPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeftPortal();

	USceneComponent* RootComponent;
	UStaticMesh Plane;
	USceneCaptureComponent2D SceneCapture;
	UBoxComponent Box;
	UArrowComponent Arrow;

private:
	bool IsSecondPortalSpawned;
	bool IsPlayer;
	bool FirstRun;
	int32 VelocityAddition;
	int32 SetIndex;
	int32 VelocityAdditionZVector;
	FVector LeftPortalForwardVector;
	FVector RightPortalForwardVector;
	FVector OriginalCollisionLocation;
	FVector RandomCollisionLocation;
	FRotator LeftPortalRotation;
	FRotator RightPortalRotation;
	FRotator PlayerRotation;
	APlayerController* PlayerController;
	AActor* CharacterRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetplayerVelocity();
	void SetPlayerRotation();
	void ResetMaxVelocity();
	void SetVelocityAccordingToForwardVector();
	void SetPortalLocationOnCall();
	void SetRenderTargetLocation();
};
