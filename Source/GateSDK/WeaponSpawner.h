// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "WeaponSpawner.generated.h"

UENUM()
enum ESpawnerType
{
	Wep_Sniper UMETA(DisplayName="Sniper"),
	Wep_Railgun UMETA(DisplayName="Railgun"),
	Wep_BFB UMETA(DisplayName="BFB"),
	Wep_SMG UMETA(DisplayName="SMG"),
	Wep_Plasma UMETA(DisplayName="Plasma"),
	Wep_Rocket UMETA(DisplayName="Rocket"),
	Wep_Shotgun UMETA(DisplayName="Shotgun"),
	Wep_BR UMETA(DisplayName="BattleRifle"),
	Wep_AR UMETA(DisplayName="AssaultRifle")
};

UCLASS()
class GATESDK_API AWeaponSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		float RotationSpeed;

	UFUNCTION()
		void StartRespawnTimer();

	UFUNCTION()
		void PickupWeapon();

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ESpawnerType> SpawnerType;

	UPROPERTY(EditAnywhere)
		float CollisionSphereRadius = 75.f;

	UPROPERTY(VisibleAnywhere)
		bool bIsPickedUp;

	UPROPERTY(VisibleAnywhere)
		float RespawnTimer;

	UPROPERTY(VisibleAnywhere)
		float CurrentRespawnTime;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SpawnerRootComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* WepMesh;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereCollider;

	UPROPERTY()
		UMaterial* BaseShortMaterial;

	UPROPERTY()
		UMaterial* BaseLongMaterial;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
