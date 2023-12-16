// Fill out your copyright notice in the Description page of Project Settings.


#include "game_PlayerController.h"


Agame_PlayerController::Agame_PlayerController()
	:forwardInput(0),
	rightInput(0),
	inputThreshold(0.2f),
	acceleration(15.f),
	walkingSpeed(500.f),
	sprintingSpeed(800.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	maxSpeed = walkingSpeed;
}

void Agame_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	playerPawn = GetPawn();
	playerCharacter = (Agame_PlayerCharacter*)playerPawn;
}

void Agame_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveOnInput(playerCharacter, forwardInput, rightInput, DeltaTime);
	UpdateStates(playerPawn);
	StoreMoveDataWhileGrounded();
	
}

void Agame_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &Agame_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &Agame_PlayerController::MoveRight);
	InputComponent->BindAxis("LookUp", this, &Agame_PlayerController::LookUp);
	InputComponent->BindAxis("LookRight", this, &Agame_PlayerController::LookRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &Agame_PlayerController::TryJumping);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &Agame_PlayerController::UpdateSprinting);
	//InputComponent->BindAction("Sprint", IE_Released, this, &Agame_PlayerController::UpdateSprinting);
}

void Agame_PlayerController::MoveOnInput(Agame_PlayerCharacter* character, float fInput, float rInput, float DeltaTime)
{
	if (character)
	{
		if (character->isGrounded) 
		{
			movementDirection = character->camera->GetForwardVector() * fInput
				+ character->camera->GetRightVector() * rInput;
		}
		else
		{
			//need stored vector and velocity before jumping ????????????????????????????????????????
			
			movementDirection = character->camera->GetForwardVector() * fInput
				+ character->camera->GetRightVector() * rInput;
				
		}



		movementDirection.Z = 0;
		movementDirection.Normalize();

		FVector targetVelocity = movementDirection * maxSpeed;
		currentVelocity = FMath::Lerp(currentVelocity, targetVelocity, acceleration * DeltaTime);

		FVector newLocation = character->GetActorLocation() + (currentVelocity * DeltaTime);
		character->SetActorLocation(newLocation, true);
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

void Agame_PlayerController::TryJumping()
{
	playerCharacter->Jump();
}

void Agame_PlayerController::UpdateSprinting()
{
	if (playerCharacter->isSprinting || forwardInput < inputThreshold)
	{
		playerCharacter->isSprinting = false;
		maxSpeed = walkingSpeed;
	}
	else
	{
		playerCharacter->isSprinting = true;
		maxSpeed = sprintingSpeed;
	}
}

void Agame_PlayerController::LookUp(float value)
{
	if (value)
	{
		UCameraComponent* cam = playerCharacter->camera;
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
		UCameraComponent* cam = playerCharacter->camera;
		cam->AddWorldRotation(FRotator(0, value, 0));
		
	}
}

void Agame_PlayerController::UpdateStates(APawn* player)
{
	//update if player is grounded
	((Agame_PlayerCharacter*)player)->isGrounded = ((Agame_PlayerCharacter*)player)->GetCharacterMovement()->IsMovingOnGround();

	//sprinting update happens on input
	
	
	//stop sprinting if stop moving
	if (forwardInput < inputThreshold)
	{
		UpdateSprinting();
	}


}

void Agame_PlayerController::StoreMoveDataWhileGrounded()
{
	if (playerCharacter->isGrounded)
	{
		stored_movementDirection = movementDirection;
		stored_currentVelocity = currentVelocity;
	}
}


