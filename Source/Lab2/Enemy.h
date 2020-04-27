// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StateMachine.h"
#include "Enemy.generated.h"

UCLASS()
class LAB2_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	//Sets the states for the enemy
	enum Enemy_States
	{
		STATE_DO_NOTHING = 0,
		CHASE_STATE,
		FLEE_STATE,
		WAIT_STATE,
		ATTACK_STATE
	};
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	Enemy_States GetCurrentState() { return m_StateMachine->GetCurrentState(); }
	void SetCurrentState(Enemy_States state) { m_StateMachine->ChangeState(state); }

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	float speed = 200.0f;

	StateMachine<Enemy_States, AEnemy>* m_StateMachine;

	//Chase state
	void State_Chase_OnEnter(void);
	void State_Chase_OnTick(float f_DeltaTime);
	void State_Chase_OnExit(void);

	//Flee state
	void State_Flee_OnEnter(void);
	void State_Flee_OnTick(float f_DeltaTime);
	void State_Flee_OnExit(void);

	//Wait State
	void State_Wait_OnEnter(void);
	void State_Wait_OnTick(float f_DeltaTime);
	void State_Wait_OnExit(void);

	//Attack State
	void State_Attack_OnEnter(void);
	void State_Attack_OnTick(float f_DeltaTime);
	void State_Attack_OnExit(void);

	float m_timeElapse = 0.0f;
	float m_BraveryValue = (rand() / (float)RAND_MAX) * 2 + 1;

};
