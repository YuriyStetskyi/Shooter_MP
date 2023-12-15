// Fill out your copyright notice in the Description page of Project Settings.


#include "game_PlayerController.h"


Agame_PlayerController::Agame_PlayerController()
	:forwardInput(0),
	rightInput(0),
	inputThreshold(0.2f),
	maxSpeed(800.f),
	acceleration(15.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void Agame_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	playerCharacter = GetPawn();
}

void Agame_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveOnInput(playerCharacter, forwardInput, rightInput, DeltaTime);
}

void Agame_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &Agame_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &Agame_PlayerController::MoveRight);
	InputComponent->BindAxis("LookUp", this, &Agame_PlayerController::LookUp);
	InputComponent->BindAxis("LookRight", this, &Agame_PlayerController::LookRight);
}

void Agame_PlayerController::MoveOnInput(AActor* pawn, float fInput, float rInput, float DeltaTime)
{
	if (pawn && (fInput || rInput))
	{
		Agame_PlayerCharacter* playerPawn = (Agame_PlayerCharacter*)pawn;
		movementDirection = playerPawn->camera->GetForwardVector() * fInput
			+ playerPawn->camera->GetRightVector() * rInput;
		movementDirection.Z = 0;
		movementDirection.Normalize();

		FVector targetVelocity = movementDirection * maxSpeed;
		currentVelocity = FMath::Lerp(currentVelocity, targetVelocity, acceleration * DeltaTime);

		FVector newLocation = pawn->GetActorLocation() + (currentVelocity * DeltaTime);
		pawn->SetActorLocation(newLocation, true);
	}
	
}

void Agame_PlayerController::MoveForward(float value)
{
	if (value)
	{
		if (FMath::Abs(value) > inputThreshold)
		{
			forwardInput = value;
		}
	}
	else
	{
		forwardInput = 0.0f;
	}
}

void Agame_PlayerController::MoveRight(float value)
{
	if (value)
	{
		if (FMath::Abs(value) > inputThreshold)
		{
			rightInput = value;
		}
	}
	else
	{
		rightInput = 0.0f;
	}
}

void Agame_PlayerController::LookUp(float value)
{
	if (value)
	{
		UCameraComponent* cam = ((Agame_PlayerCharacter*)playerCharacter)->camera;
		float temp = cam->GetRelativeRotation().Pitch + value;
		if (temp <= 90 && temp >= -90)
		{
			cam->AddLocalRotation(FRotator(value, 0, 0)); //move camera. not whole obeject. otherwise wont work
		}
	}
	
}

void Agame_PlayerController::LookRight(float value)
{
	if (value)
	{
		UCameraComponent* cam = ((Agame_PlayerCharacter*)playerCharacter)->camera;
		cam->AddWorldRotation(FRotator(0, value, 0));
		
	}
}


