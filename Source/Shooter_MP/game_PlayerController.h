// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "game_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
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
	APawn* playerCharacter;
	void MoveOnInput(AActor* pawn, float fInput, float rInput, float DeltaTime);
	void MoveForward(float value);
	void MoveRight(float value);


	//camera
	void LookUp(float value);
	void LookRight(float value);

private:

	//movement
	float forwardInput;
	float rightInput;
	float inputThreshold;
	FVector movementDirection;
	
	float maxSpeed;
	float acceleration;
	FVector currentVelocity;
};
