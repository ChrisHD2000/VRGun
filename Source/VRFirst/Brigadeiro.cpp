// Fill out your copyright notice in the Description page of Project Settings.


#include "Brigadeiro.h"

// Sets default values
ABrigadeiro::ABrigadeiro()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!BrigadeiroMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		BrigadeiroMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BrigadeiroMovementComponent"));
		BrigadeiroMovementComponent->SetUpdatedComponent(CollisionComponent);
		BrigadeiroMovementComponent->InitialSpeed = 100.0f;
		BrigadeiroMovementComponent->MaxSpeed = 100.0f;
		BrigadeiroMovementComponent->bRotationFollowsVelocity = true;
		BrigadeiroMovementComponent->bShouldBounce = true;
		BrigadeiroMovementComponent->Bounciness = 0.3f;
		BrigadeiroMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!SparklesMeshComponent)
	{
		SparklesMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SparklesMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Meshes/SparklesMesh.SparklesMesh"));
		if (Mesh.Succeeded())
		{
			SparklesMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	if (!BrigadeiroMeshComponent)
	{
		BrigadeiroMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrigadeiroMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Meshes/BrigadeiroMesh.BrigadeiroMesh"));
		if (Mesh.Succeeded())
		{
			BrigadeiroMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	SparklesMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	SparklesMeshComponent->SetupAttachment(RootComponent);
	BrigadeiroMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	BrigadeiroMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABrigadeiro::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrigadeiro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function that initializes the projectile's velocity in the shoot direction.
void ABrigadeiro::FireInDirection(const FVector& ShootDirection)
{
	BrigadeiroMovementComponent->Velocity = ShootDirection * BrigadeiroMovementComponent->InitialSpeed;
}