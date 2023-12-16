// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "game_playerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "game_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_MP_API Agame_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	Agame_PlayerController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//movement
	virtual void SetupInputComponent() override;
	APawn* playerPawn;
	Agame_PlayerCharacter* playerCharacter;
	void MoveOnInput(Agame_PlayerCharacter* character, float fInput, float rInput, float DeltaTime);
	void MoveForward(float value);
	void MoveRight(float value);
	void TryJumping();
	void UpdateSprinting();

	//camera
	void LookUp(float value);
	void LookRight(float value);

	void UpdateStates(APawn* player);
	void StoreMoveDataWhileGrounded();


private:

	//movement
	float forwardInput;
	float rightInput;
	float inputThreshold;
	FVector movementDirection;
	FVector stored_movementDirection;
	
	float maxSpeed;
	float acceleration;
	FVector currentVelocity;
	FVector stored_currentVelocity;

	float walkingSpeed;
	float sprintingSpeed;

};
