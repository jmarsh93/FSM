// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "CubePlayer.generated.h"

UCLASS()
class LAB2_API ACubePlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACubePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float speed = 500.0f;
	FVector MovementInput;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Explosion();

	float health = 500.0f;
	float damageCooldown = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = Default)
		UMaterialInterface* HurtMaterial;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = Default)
		UMaterialInterface* UnHurtMaterial;

	void TakeDamage(float Damage);
};
