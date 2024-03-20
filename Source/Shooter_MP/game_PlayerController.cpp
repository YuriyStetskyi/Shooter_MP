// Copyright Epic Games, Inc. All Rights Reserved.

#pragma optimize("", off)
#include "game_PlayerController.h"


Agame_PlayerController::Agame_PlayerController()
	:forwardInput(0),
	rightInput(0),
	inputThreshold(0.2f),
	acceleration(15.f),
	walkingSpeed(500.f),
	sprintingSpeed(800.0f),
	tickInProgress(0),
	currentFPS(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	maxSpeed = walkingSpeed;
	bReplicates = true;
}

void Agame_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	GEngine->bUseFixedFrameRate = false; //in case you changed FPS during testing
	playerPawn = GetPawn();
	playerCharacter = (Agame_PlayerCharacter*)playerPawn;

	if(playerCharacter && playerCharacter->bodyMesh)
		playerCharacter->bodyMesh->SetIsReplicated(true);

	if(playerCharacter && playerCharacter->gunMesh)
		playerCharacter->gunMesh->SetIsReplicated(true);	
}

void Agame_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority() && playerCharacter)
	{
		Multicast_MoveOnInput(playerCharacter, forwardInput, rightInput, DeltaTime);
		Multicast_RotateCameraOnInput(playerCharacter, cameraPitch, cameraYaw, DeltaTime);
		Multicast_UpdateStates(playerCharacter, playerCharacter->isSprinting, forwardInput);
	}
	else if (!HasAuthority() && playerCharacter)
	{
		MoveOnInput(playerCharacter, forwardInput, rightInput, DeltaTime);
		Server_MoveOnInput(playerCharacter, forwardInput, rightInput, DeltaTime);

		RotateCameraOnInput(playerCharacter, cameraPitch, cameraYaw, DeltaTime);
		Server_RotateCameraOnInput(playerCharacter, cameraPitch, cameraYaw, DeltaTime);

		UpdateStates(playerCharacter, playerCharacter->isSprinting, forwardInput);
		Server_UpdateStates(playerCharacter, playerCharacter->isSprinting, forwardInput);
	}

	ImproveJump(playerCharacter);
	StoreMoveDataWhileGrounded(playerCharacter);
	
	//testing
	Enable_Framerate_Changer();

	if (HasAuthority() && IsLocalController())
	{
		FixedTick(40, DeltaTime);
	}

	CalculateFPS(DeltaTime);

}

void Agame_PlayerController::FixedTick(float TickRate, float DeltaTime)
{
	if (FixedTick_Logic(TickRate, DeltaTime))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Emerald, TEXT("tick once per fixed frame. if fixed frame is 1 - tick once a second"), true, FVector2D(1.5, 1.5));


	}
}

bool Agame_PlayerController::FixedTick_Logic(float TickRate, float DeltaTime)
{
	float fixedDeltaTime = 1 / TickRate; //0,0333... for TickRate = 30

	tickInProgress += DeltaTime;
	if (tickInProgress > fixedDeltaTime)
	{
		tickInProgress = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void Agame_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->SetIsReplicated(true);

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
		movementDirection = character->camera->GetForwardVector() * fInput
			+ character->camera->GetRightVector() * rInput;
		movementDirection.Z = 0;
		movementDirection.Normalize();

		FVector targetVelocity = movementDirection * maxSpeed;
		currentVelocity = FMath::Lerp(currentVelocity, targetVelocity, acceleration * DeltaTime);

		FVector newLocation; 
		if (character->isHittingStuff && FVector::DotProduct(movementDirection, character->hitNormal) <= 0)
		{
			FVector slideVelocity = currentVelocity - (character->hitNormal * FVector::DotProduct(currentVelocity, character->hitNormal));
			newLocation = character->GetActorLocation() + (slideVelocity * DeltaTime);
		}
		else
		{
			newLocation = character->GetActorLocation() + (currentVelocity * DeltaTime);
		}

		character->SetActorLocation(newLocation, true);
	}
	
}

void Agame_PlayerController::Multicast_MoveOnInput_Implementation(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime)
{
	MoveOnInput(playerChar, fInput, rInput, DeltaTime);
}

void Agame_PlayerController::Server_MoveOnInput_Implementation(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime)
{
	MoveOnInput(playerChar, fInput, rInput, DeltaTime);
}



void Agame_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(Agame_PlayerController, forwardInput);
	DOREPLIFETIME(Agame_PlayerController, rightInput);
	DOREPLIFETIME(Agame_PlayerController, cameraPitch);
	DOREPLIFETIME(Agame_PlayerController, cameraYaw);
	DOREPLIFETIME(Agame_PlayerController, maxSpeed);
	DOREPLIFETIME(Agame_PlayerController, playerPawn);
	DOREPLIFETIME(Agame_PlayerController, playerCharacter);
	DOREPLIFETIME(Agame_PlayerController, isSprinting);
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
	timeOfJumpPress = GetWorld()->GetTimeSeconds();
}

void Agame_PlayerController::UpdateSprinting()
{
	if (playerCharacter)
	{
		if (playerCharacter->isSprinting || forwardInput < inputThreshold)
		{
			playerCharacter->isSprinting = false;
		}
		else
		{
 			playerCharacter->isSprinting = true;
		}
	}
}

void Agame_PlayerController::UpdateSpeed(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput)
{
	if (playerChar)
	{
		if (is_Sprinting && fInput > inputThreshold)
		{
			float spd = ((Agame_PlayerController*)(playerChar->GetOwner()))->maxSpeed;
			((Agame_PlayerController*)(playerChar->GetOwner()))->maxSpeed = sprintingSpeed;
		}
		else
		{
			if (playerCharacter)
			{
				playerChar->isSprinting = false;
			}
			float spd = ((Agame_PlayerController*)(playerChar->GetOwner()))->maxSpeed;
			((Agame_PlayerController*)(playerChar->GetOwner()))->maxSpeed = walkingSpeed;
		}
	}
}

void Agame_PlayerController::OnFocusChanged(bool newFocusState)
{
	forwardInput = 0;
	rightInput = 0;
	movementDirection = FVector::ZeroVector;
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Emerald, TEXT("ALT TABBED"), true, FVector2D(1.5, 1.5));
}

void Agame_PlayerController::LookUp(float value)
{
	if (value)
	{
		cameraPitch = value;
	}
	else
	{
		cameraPitch = 0.0f;
	}
}

void Agame_PlayerController::LookRight(float value)
{
	if (value)
	{
		cameraYaw = value;
	}
	else
	{
		cameraYaw = 0.0f;
	}
}

void Agame_PlayerController::RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime)
{

	
	if (pitch)
	{
		float result = playerChar->camera->GetRelativeRotation().Pitch + pitch;
		if (result <= 90 && result >= -90)
		{
			playerChar->camera->AddLocalRotation(FRotator(pitch, 0, 0));	//move camera. not whole obeject. otherwise wont work
		}

		if (playerChar->gunMesh)
		{
			FRotator rot(playerChar->camera->GetRelativeRotation().Pitch, 0, 0);
			playerChar->gunMesh->AddLocalRotation(rot);
		}
	}

	if (yaw)
	{
		playerChar->camera->AddWorldRotation(FRotator(0, yaw, 0));

		//move body and gun as well. -90 because of relativity of rotation, its kinda crutch
		
		if(playerChar->bodyMesh)
			playerChar->bodyMesh->SetWorldRotation(FRotator(0, playerChar->camera->GetRelativeRotation().Yaw - 90, 0));
		

	}


}

void Agame_PlayerController::Multicast_RotateCameraOnInput_Implementation(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime)
{
	RotateCameraOnInput(playerChar, pitch, yaw, DeltaTime);
}

void Agame_PlayerController::Server_RotateCameraOnInput_Implementation(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime)
{
	RotateCameraOnInput(playerChar, pitch, yaw, DeltaTime);
}

void Agame_PlayerController::UpdateStates(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput)
{
	//update if player is grounded
	if (playerChar)
	{
		playerChar->isGrounded = playerChar->GetCharacterMovement()->IsMovingOnGround();
	}

	//sprinting update happens on input or if player stops
	if (playerChar)
	{
		isSprinting = is_Sprinting;
	}
	
	//stop sprinting if stop moving
	UpdateSpeed(playerChar, is_Sprinting, fInput);


}

void Agame_PlayerController::Multicast_UpdateStates_Implementation(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput)
{
	UpdateStates(playerChar, is_Sprinting, fInput);
}

void Agame_PlayerController::Server_UpdateStates_Implementation(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput)
{
	UpdateStates(playerChar, is_Sprinting, fInput);
}

void Agame_PlayerController::StoreMoveDataWhileGrounded(Agame_PlayerCharacter* playerChar)
{
	if (playerChar)
	{
		if (playerChar->isGrounded)
		{
			stored_movementDirection = movementDirection;
			stored_currentVelocity = currentVelocity;
		}
	}
}

void Agame_PlayerController::ImproveJump(Agame_PlayerCharacter* playerChar)
{
	if (playerChar)
	{
		if (playerChar->isGrounded)
		{
			timeOfLanding = GetWorld()->GetTimeSeconds();
		}
		else
		{
			timeOfLanding = 0.0f;
		}

		if (timeOfLanding - timeOfJumpPress < 0.15f && timeOfLanding - timeOfJumpPress > 0 && timeOfJumpPress != 0)
		{
			TryJumping();
		}
	}
}

void Agame_PlayerController::CalculateFPS(float DeltaTime)
{
	currentFPS = 1 / DeltaTime;
}


void Agame_PlayerController::Enable_Framerate_Changer()
{
	if (IsInputKeyDown(EKeys::I) && IsInputKeyDown(EKeys::LeftControl) && GEngine->GetMaxFPS() != 15)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("FPS CHANGED TO 15"), true, FVector2D(2,2));
		GEngine->SetMaxFPS(15);

	}

	if (IsInputKeyDown(EKeys::O) && IsInputKeyDown(EKeys::LeftControl) && GEngine->GetMaxFPS() != 30)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("FPS CHANGED TO 30"), true, FVector2D(2, 2));
		GEngine->SetMaxFPS(30.0);

	}

	if (IsInputKeyDown(EKeys::K) && IsInputKeyDown(EKeys::LeftControl) && GEngine->GetMaxFPS() != 60)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, TEXT("FPS CHANGED TO 60"), true, FVector2D(2, 2));
		GEngine->SetMaxFPS(60.0);
	}

	if (IsInputKeyDown(EKeys::L) && IsInputKeyDown(EKeys::LeftControl) && GEngine->GetMaxFPS() != 144)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("FPS CHANGED TO 144"), true, FVector2D(2, 2));
		GEngine->SetMaxFPS(144.0);
	}

	if (IsInputKeyDown(EKeys::M) && IsInputKeyDown(EKeys::LeftControl) && GEngine->GetMaxFPS() != 999)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("FPS UNCAPPED"), true, FVector2D(2, 2));
		GEngine->SetMaxFPS(999.0);
	}
}




