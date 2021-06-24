// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"

// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(SpawnerRootComponent);

	WepMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WepMesh->SetupAttachment(SpawnerRootComponent);
	WepMesh->SetWorldScale3D(FVector(.25f, .25f, .25f));

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollider->SetupAttachment(SpawnerRootComponent);
	SphereCollider->SetSphereRadius(25.f);
	DrawDebugSphere(GetWorld(), FVector(SphereCollider->GetComponentLocation()), SphereCollider->GetScaledSphereRadius(), 16, FColor::Purple, true, -1.f);

}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//decide wep mesh based on enum here
}

// Called every frame
void AWeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NewWepMeshRotation = GetActorRotation();
	NewWepMeshRotation.Yaw += DeltaTime * RotationSpeed;
	FVector WepMeshPosition;
	WepMesh->SetWorldRotation(NewWepMeshRotation);
}

void AWeaponSpawner::StartRespawnTimer()
{

}

void AWeaponSpawner::PickupWeapon()
{

}