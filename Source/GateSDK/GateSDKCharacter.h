// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GateSDKCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AGateSDKCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;



public:
	AGateSDKCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AGateSDKProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	//UPROPERTY(VisibleAnywhere)
	//UCapsuleComponent* TriggerComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALeftPortal> LeftPortalClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARightPortal> RightPortalClass;
	
	UPROPERTY(VisibleAnywhere)
	class ALeftPortal* StoredLeftPortal;

	UPROPERTY(VisibleAnywhere)
	class ARightPortal* StoredRightPortal;

private:
	UPROPERTY(VisibleAnywhere)
	FVector SavedPlayerVelocity;

	UPROPERTY(VisibleAnywhere)
	FVector LeftPortalLocation;

	UPROPERTY(VisibleAnywhere)
	FVector RightPortalLocation;

	UPROPERTY(VisibleAnywhere)
	FVector LeftPortalForwardVector;

	UPROPERTY(VisibleAnywhere)
	FVector RightPortalForwardVector;

	UPROPERTY(VisibleAnywhere)
	FRotator LeftPortalRotation;

	UPROPERTY(VisibleAnywhere)
	FRotator RightPortalRotation;

	UPROPERTY(VisibleAnywhere)
	bool bLeftPortalSpawned;

	UPROPERTY(VisibleAnywhere)
	bool bRightPortalSpawned;

	UPROPERTY(VisibleAnywhere)
	bool bNewlySpawnedLeftPortal;

	UPROPERTY(VisibleAnywhere)
	bool bNewlySpawnedRightPortal;

	UPROPERTY(VisibleAnywhere)
	bool bCanTeleport = true;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	UFUNCTION()
	void SpawnLeftPortal();

	UFUNCTION()
	void SpawnRightPortal();

	UFUNCTION()
	void DestroyLeftPortal();

	UFUNCTION()
	void DestroyRightPortal();

	UFUNCTION()
	void StorePlayerVelocity();

	UFUNCTION()
	void LeftPortalCollisionCheck();

	UFUNCTION()
	void RightPortalCollisionCheck();

	//UFUNCTION()
	//void OnBeginActorOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION()
	FVector GetLeftPortalForwardVector();
	
	UFUNCTION()
	FVector GetRightPortalForwardVector();

	UFUNCTION()
	bool GetCanTeleport();

	UFUNCTION()
	void SetCanTeleport(bool bCanTele);

	UFUNCTION()
	FVector GetRightPortalLocation();

	UFUNCTION()
	FVector GetLeftPortalLocation();

};

