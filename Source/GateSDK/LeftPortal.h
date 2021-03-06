// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GateSDKCharacter.h"
#include "LeftPortal.generated.h"

UCLASS()
class GATESDK_API ALeftPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeftPortal();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PortalRootComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Plane;

	UPROPERTY(VisibleAnywhere)
	USceneCaptureComponent2D* SceneCapture;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY()
	UMaterial* RightPortalNotSpawnedMat;

	UPROPERTY()
	UMaterial* RightPortalMat;


//private:

	UPROPERTY(VisibleAnywhere)
	bool IsSecondPortalSpawned;
	
	UPROPERTY(VisibleAnywhere)
	bool IsPlayer;
	
	UPROPERTY(VisibleAnywhere)
	bool FirstRun;
	
	UPROPERTY(VisibleAnywhere)
	int32 VelocityAddition;
	
	UPROPERTY(VisibleAnywhere)
	int32 SetIndex;

	UPROPERTY(VisibleAnywhere)
	int32 VelocityAdditionZVector;
	
	UPROPERTY(VisibleAnywhere)
	FVector LeftPortalForwardVector;
	
	UPROPERTY(VisibleAnywhere)
	FVector RightPortalForwardVector;
	
	UPROPERTY(VisibleAnywhere)
	FVector OriginalCollisionLocation;
	
	UPROPERTY(VisibleAnywhere)
	FVector RandomCollisionLocation;
	
	UPROPERTY(VisibleAnywhere)
	FRotator LeftPortalRotation;
	
	UPROPERTY(VisibleAnywhere)
	FRotator RightPortalRotation;
	
	UPROPERTY(VisibleAnywhere)
	FRotator PlayerRotation;
	
	UPROPERTY(VisibleAnywhere)
	APlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere)
	AGateSDKCharacter* CharacterRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetPlayerVelocity();

	UFUNCTION()
	void SetPlayerRotation();

	UFUNCTION()
	void ResetMaxVelocity();

	UFUNCTION()
	void SetVelocityAccordingToForwardVector();
	
	UFUNCTION()
	void SetPortalLocationOnCall();
	
	UFUNCTION()
	void SetRenderTargetRotation();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};
