// Fill out your copyright notice in the Description page of Project Settings.

#include "CubePlayer.h"
#include "Enemy.h"
#include "Engine/GameEngine.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACubePlayer::ACubePlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Connecting Components Again
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(-1000.0f, 0.0f, 1000.0f));
	Camera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));


}

// Called when the game starts or when spawned
void ACubePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (damageCooldown > 0)
	{
		damageCooldown -= DeltaTime;
		if (damageCooldown <= 0)
		{
			VisibleComponent->SetMaterial(0, UnHurtMaterial);
		}
	}

	if (!MovementInput.IsZero())
	{
		MovementInput.Normalize();
		FVector NewLocation = GetActorLocation() + (MovementInput * speed * DeltaTime);
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void ACubePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveY", this, &ACubePlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveX", this, &ACubePlayer::MoveForward);
	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &ACubePlayer::Explosion);
}

void ACubePlayer::TakeDamage(float Damage)
{
	if (damageCooldown <= 0)
	{
		damageCooldown = 1.0f;
		health -= Damage;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Health: %f"), health));
			VisibleComponent->SetMaterial(0, HurtMaterial);
		}
	}
}


void ACubePlayer::MoveForward(float AxisValue) 
{
	MovementInput.X = AxisValue;
}

void ACubePlayer::MoveRight(float AxisValue)
{
	MovementInput.Y = AxisValue;
}

void ACubePlayer::Explosion()
{
	TArray<FHitResult> OutHits;
	FVector location = GetActorLocation();
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(250.f);

	DrawDebugSphere(GetWorld(), location, ExplosionSphere.GetCapsuleRadius(), 50, FColor::Red, true);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, location, location, FQuat::Identity, ECC_WorldStatic, ExplosionSphere);

	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>((Hit.GetActor()->GetRootComponent()));
			if (Mesh)
			{
				Mesh->AddRadialImpulse(location, 500.0f, 2000.0f, ERadialImpulseFalloff::RIF_Linear, true);
			}

			AEnemy* enemy = Cast<AEnemy>(Hit.GetActor());
			if (enemy)
			{
				enemy->SetCurrentState(AEnemy::FLEE_STATE);
			}
		}
	}
}
