// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeftPortal.generated.h"

UCLASS()
class GATESDK_API ALeftPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeftPortal();

private:
	bool IsSecondPortalSpawned;
	bool IsPlayer;
	bool FirstRun;
	int VelocityAddition;
	FVector LeftPortalForwardVector;
	FVector RightPortalForwardVector;
	FVector OriginalCollisionLocation;
	FVector RandomCollisionLocation;
	FRotator LeftPortalRotation;
	FRotator RightPortalRotation;
	FRotator PlayerRotation;
	APlayerController PlayerController;
	//gatesdkreference;

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
