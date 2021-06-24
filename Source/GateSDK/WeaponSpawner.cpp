// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"

// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial>BaseMatLong(TEXT("Material'/Game/Materials/SpawnerLongMat.SpawnerLongMat'"));
	if (BaseMatLong.Succeeded())
	{
		BaseLongMaterial = BaseMatLong.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>BaseMatShort(TEXT("Material'/Game/Materials/SpawnerShortMat.SpawnerShortMat'"));
	if (BaseMatShort.Succeeded())
	{
		BaseShortMaterial = BaseMatShort.Object;
	}

	SpawnerRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(SpawnerRootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SpawnerMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (SpawnerMesh.Succeeded())
	{
		BaseMesh->SetStaticMesh(SpawnerMesh.Object);
	}

	WepMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WepMesh->SetupAttachment(SpawnerRootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GunMesh(TEXT("StaticMesh'/Game/Meshes/SM_Gun.SM_Gun'"));
	if (GunMesh.Succeeded())
	{
		WepMesh->SetStaticMesh(GunMesh.Object);
	}

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollider->SetupAttachment(SpawnerRootComponent);
	SphereCollider->SetSphereRadius(CollisionSphereRadius);
	DrawDebugSphere(GetWorld(), FVector(SphereCollider->GetComponentLocation()), SphereCollider->GetScaledSphereRadius(), 16, FColor::Purple, true, -1.f);
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnerType == ESpawnerType::Wep_Sniper)
	{
		BaseMesh->SetMaterial(0, BaseLongMaterial);
		RespawnTimer = 120.f; //2 min
	}
	else if (SpawnerType == ESpawnerType::Wep_BFB)
	{
		BaseMesh->SetMaterial(0, BaseLongMaterial);
		RespawnTimer = 150.f; //2 min 30 sec
	}
	else if (SpawnerType == ESpawnerType::Wep_Railgun)
	{
		BaseMesh->SetMaterial(0, BaseLongMaterial);
		RespawnTimer = 150.f; //2 min 30 sec
	}
	else if (SpawnerType == ESpawnerType::Wep_Plasma)
	{
		BaseMesh->SetMaterial(0, BaseShortMaterial);
		RespawnTimer = 90.f; //1 min 30 sec
	}
	else if (SpawnerType == ESpawnerType::Wep_SMG)
	{
		BaseMesh->SetMaterial(0, BaseShortMaterial);
		RespawnTimer = 60.f; //1 min
	}
	else if (SpawnerType == ESpawnerType::Wep_Rocket)
	{
		BaseMesh->SetMaterial(0, BaseLongMaterial);
		RespawnTimer = 150.f; //2 min 30 sec
	}
	else if (SpawnerType == ESpawnerType::Wep_Shotgun)
	{
		BaseMesh->SetMaterial(0, BaseLongMaterial);
		RespawnTimer = 120.f; //2 min
	}
	else if (SpawnerType == ESpawnerType::Wep_BR)
	{
		BaseMesh->SetMaterial(0, BaseShortMaterial);
		RespawnTimer = 60.f; //1 min
	}
	else if (SpawnerType == ESpawnerType::Wep_AR)
	{
		BaseMesh->SetMaterial(0, BaseShortMaterial);
		RespawnTimer = 30.f; //30 sec
	}

	DrawDebugSphere(GetWorld(), FVector(SphereCollider->GetComponentLocation()), SphereCollider->GetScaledSphereRadius(), 16, FColor::Purple, true, -1.f);
}

// Called every frame
void AWeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewWepMeshRotation = WepMesh->GetComponentRotation();
	NewWepMeshRotation.Yaw += DeltaTime * RotationSpeed;
	FVector NewWepMeshPosition = WepMesh->GetComponentLocation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewWepMeshPosition.Z += DeltaHeight * 10.f;
	WepMesh->SetWorldLocationAndRotation(NewWepMeshPosition, NewWepMeshRotation);
}

void AWeaponSpawner::StartRespawnTimer()
{

}

void AWeaponSpawner::PickupWeapon()
{

}