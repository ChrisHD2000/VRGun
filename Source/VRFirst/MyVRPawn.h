// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "MyVRPawn.generated.h"

UCLASS()
class VRFIRST_API AMyVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void moveForward(float valY);
	void moveRight(float valX);

	USceneComponent* shootSocket;

	FVector offset = FVector::ZeroVector;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
		USceneComponent* VRTrackingCenter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
		UCameraComponent* Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
		UTextRenderComponent* outputText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
		class UMotionControllerComponent* RightController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
		class UMotionControllerComponent* LeftController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
		UStaticMeshComponent* RayGun;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
		UStaticMeshComponent* LeftCone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
		UStaticMeshComponent* RightMesh;

	UFUNCTION()
		void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABrigadeiro> BrigadeiroClass;

	UPROPERTY(EditDefaultsOnly, Category = Colision)
		USphereComponent* sphere;

};
