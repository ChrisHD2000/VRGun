// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRPawn.h"
#include "Brigadeiro.h"
#include "DrawDebugHelpers.h"
#include "IXRTrackingSystem.h"
#include "Engine.h"

// Sets default values
AMyVRPawn::AMyVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AutoReceiveInput = EAutoReceiveInput::Player0;
	// Subobjects

	VRTrackingCenter = CreateDefaultSubobject<USceneComponent>(TEXT("VRTrackingCenter"));
	Head = CreateDefaultSubobject<UCameraComponent>(TEXT("Head"));
	outputText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("OutputText"));

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftCone"));
	RayGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RayGun"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightMesh"));

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphere->InitSphereRadius(40.0f);
	sphere->SetCollisionProfileName(TEXT("Pawn"));
	sphere->SetupAttachment(Head);

	shootSocket = CreateDefaultSubobject<USceneComponent>(TEXT("shootSocket"));

	// Attachments

	Head->SetupAttachment(VRTrackingCenter);
	outputText->SetupAttachment(Head);
	outputText->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	outputText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	outputText->SetText(TEXT("Hola"));

	LeftController->SetupAttachment(VRTrackingCenter);
	RayGun->SetupAttachment(LeftController);
	LeftCone->SetupAttachment(RayGun);

	RightController->SetupAttachment(VRTrackingCenter);
	RightMesh->SetupAttachment(RightController);

	shootSocket->SetupAttachment(RayGun);
	shootSocket->AddLocalOffset(FVector(22.f, 0.f, 6.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> gunObj(TEXT("/Game/Meshes/raygun.raygun"));
	if (gunObj.Succeeded()) {RayGun->SetStaticMesh(gunObj.Object);}

}

// Called when the game starts or when spawned
void AMyVRPawn::BeginPlay()
{
	Super::BeginPlay();
	Head->bLockToHmd = true;
}

// Called every frame
void AMyVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat hmdRotation;
	FVector hmdLocationOffset;
	GEngine->XRSystem->GetCurrentPose(IXRTrackingSystem::HMDDeviceId, hmdRotation, hmdLocationOffset);
	SetActorLocation(GetActorLocation() + offset.RotateAngleAxis(hmdRotation.Rotator().Yaw - 90.f, FVector(0.f, 0.f, 1.f)));
}

// Called to bind functionality to input
void AMyVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyVRPawn::Fire);
	PlayerInputComponent->BindAxis("Move_Forward", this, &AMyVRPawn::moveForward);
	PlayerInputComponent->BindAxis("Move_Right", this, &AMyVRPawn::moveRight);
}

void AMyVRPawn::moveForward(float valY)
{
	if (valY != 0)
		offset.Y = valY;
}

void AMyVRPawn::moveRight(float valX)
{
	if (valX != 0)
		offset.X = -valX;
}

void AMyVRPawn::Fire()
{
	// Attempt to fire a projectile.
	if (BrigadeiroClass)
	{
		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(20.0f, 0.0f, 6.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = shootSocket->GetComponentLocation();

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = LeftController->GetComponentRotation();
		MuzzleRotation.Pitch += 0.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ABrigadeiro* Projectile = World->SpawnActor<ABrigadeiro>(BrigadeiroClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}