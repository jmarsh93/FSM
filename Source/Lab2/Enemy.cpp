// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Engine/GameEngine.h"
#include "ConstructorHelpers.h"
#include "CubePlayer.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Wedge(TEXT("/Game/StarterContent/Shapes/Shape_Wedge_A.Shape_Wedge_A"));

	Mesh = Cast<UStaticMeshComponent>(RootComponent);
	Mesh->SetStaticMesh(Wedge.Object);
	Mesh->SetSimulatePhysics(true);

	//StateMachine setup and register
	m_StateMachine = new StateMachine<Enemy_States, AEnemy>(this, STATE_DO_NOTHING);

	m_StateMachine->RegisterState(CHASE_STATE, &AEnemy::State_Chase_OnEnter, &AEnemy::State_Chase_OnTick, &AEnemy::State_Chase_OnExit);
	m_StateMachine->RegisterState(FLEE_STATE, &AEnemy::State_Flee_OnEnter, &AEnemy::State_Flee_OnTick, &AEnemy::State_Flee_OnExit);
	m_StateMachine->RegisterState(WAIT_STATE, &AEnemy::State_Wait_OnEnter, &AEnemy::State_Wait_OnTick, &AEnemy::State_Wait_OnExit);
	m_StateMachine->RegisterState(ATTACK_STATE, &AEnemy::State_Attack_OnEnter, &AEnemy::State_Attack_OnTick, &AEnemy::State_Attack_OnExit);

	m_StateMachine->ChangeState(CHASE_STATE);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_StateMachine->Tick(DeltaTime);

	FVector toPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();

	toPlayer.Normalize();

	FVector NewLocation = GetActorLocation() + (toPlayer * speed * DeltaTime);
	SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::State_Chase_OnEnter(void)
{
}

void AEnemy::State_Chase_OnTick(float f_DeltaTime)
{
	FVector toPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	if (toPlayer.Size() < 200.0f)
	{
		m_StateMachine->ChangeState(ATTACK_STATE);
	}

	toPlayer.Normalize();

	FVector NewLocation = GetActorLocation() + (toPlayer * speed * f_DeltaTime);
	SetActorLocation(NewLocation);
}

void AEnemy::State_Chase_OnExit(void)
{
}

void AEnemy::State_Flee_OnEnter(void)
{
}

void AEnemy::State_Flee_OnTick(float f_DeltaTime)
{
	FVector toPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	toPlayer.Normalize();
	FVector NewLocation = GetActorLocation() + (toPlayer * speed * f_DeltaTime);

	SetActorLocation(NewLocation);

	m_timeElapse += f_DeltaTime;
	if (m_timeElapse > m_BraveryValue)
	{
		m_timeElapse = 0.0f;
		m_StateMachine->ChangeState(CHASE_STATE);
	}
}

void AEnemy::State_Flee_OnExit(void)
{
}

void AEnemy::State_Wait_OnEnter(void)
{
}

void AEnemy::State_Wait_OnTick(float f_DeltaTime)
{

}

void AEnemy::State_Wait_OnExit(void)
{
}

void AEnemy::State_Attack_OnEnter(void)
{
}

void AEnemy::State_Attack_OnTick(float f_DeltaTime)
{
	FVector toPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	if (toPlayer.Size() > 200.0f)
	{
		m_StateMachine->ChangeState(CHASE_STATE);
	}
	else
	{
		ACubePlayer* player = Cast<ACubePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		player->TakeDamage(10.0f);
	}
}

void AEnemy::State_Attack_OnExit(void)
{
}
