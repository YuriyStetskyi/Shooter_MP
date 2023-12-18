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
	GEngine->bUseFixedFrameRate = false; //in case you changed FPS during testing
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
	
	
	//testing
	Enable_Framerate_Changer();
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
}

void Agame_PlayerController::MoveOnInput(Agame_PlayerCharacter* character, float fInput, float rInput, float DeltaTime)
{
	if (character)
	{
		//need stored vector and velocity before jumping ????????????????????????????????????????
		//change air movement when not grounded in the future HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		movementDirection = character->camera->GetForwardVector() * fInput
			+ character->camera->GetRightVector() * rInput;
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

void Agame_PlayerController::Enable_Framerate_Changer()
{
	if (IsInputKeyDown(EKeys::I) && IsInputKeyDown(EKeys::LeftControl) && GEngine->FixedFrameRate != 15)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("FPS CHANGED TO 15"), true, FVector2D(2,2));
		GEngine->FixedFrameRate = 15.0;
		GEngine->bUseFixedFrameRate = true;
	}

	if (IsInputKeyDown(EKeys::O) && IsInputKeyDown(EKeys::LeftControl) && GEngine->FixedFrameRate != 30)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("FPS CHANGED TO 30"), true, FVector2D(2, 2));
		GEngine->FixedFrameRate = 30.0;
		GEngine->bUseFixedFrameRate = true;
	}

	if (IsInputKeyDown(EKeys::P) && IsInputKeyDown(EKeys::LeftControl) && GEngine->FixedFrameRate != 60)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, TEXT("FPS CHANGED TO 60"), true, FVector2D(2, 2));
		GEngine->FixedFrameRate = 60.0;
		GEngine->bUseFixedFrameRate = true;
	}

	if (IsInputKeyDown(EKeys::K) && IsInputKeyDown(EKeys::LeftControl) && GEngine->FixedFrameRate != 144)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("FPS CHANGED TO 144"), true, FVector2D(2, 2));
		GEngine->FixedFrameRate = 144.0;
		GEngine->bUseFixedFrameRate = true;
	}

	if (IsInputKeyDown(EKeys::L) && IsInputKeyDown(EKeys::LeftControl) && GEngine->FixedFrameRate != 999)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("FPS UNCAPPED"), true, FVector2D(2, 2));
		GEngine->FixedFrameRate = 999.0;
		GEngine->bUseFixedFrameRate = false;
	}
}




