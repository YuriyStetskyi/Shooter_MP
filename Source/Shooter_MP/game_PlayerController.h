// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "game_playerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "testing_General.h"
#include "Slate/SceneViewport.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"
#include <string>
#include "Net/UnrealNetwork.h"
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

	// OVERLOADED CRUCIAL METHODS
	/////////////////////////////////////////////////////////////////////

	/** Constructs an instance of object */
	Agame_PlayerController();

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Should be called inside Tick method.
	   Used to implement stable multiplayer physics, movement etc.
	   Called at a stable tickrate (if tickrate is lower than current framerate). 
	   e.g: if TickRate is 30 - everything inside Fixed tick will be called 30 times per second (if fps is > 30) 
	   @param TickRate - rate (in fps) at which fixed tick will occur */
	void FixedTick(float TickRate, float DeltaTime);

	/* Should be called inside FixedTick method.
	   Calculates if FixedTick should be called this Tick.
	   @param TickRate - rate (in fps) at which fixed tick will occur
	   @return True if FixedTick should be called this frame*/
	bool FixedTick_Logic(float TickRate, float DeltaTime);

	/* When this > DeltaTime, FixedTick will return true, and reset this to 0 */
	float tickInProgress;

	// MOVEMENT
	/////////////////////////////////////////////////////////////////////

	/*  Called to bind functionality to input */
	virtual void SetupInputComponent() override;

	/* Pawn posessed by current controller */
	UPROPERTY(Replicated)
	APawn* playerPawn;

	/* Pawn (cast to Agame_PlayerCharacter) posessed by current controller */
	UPROPERTY(Replicated)
	Agame_PlayerCharacter* playerCharacter;

	/** Handles player character movement
		@param fInput - forward input read from keyboard/controller (W key/ S key)
		@param rInput - right input read from keyboard/controller (D key/ A key) */
	void MoveOnInput(Agame_PlayerCharacter* character, float fInput, float rInput, float DeltaTime);

	/** Multicast version of MoveOnInput
		Actual implementation is inside Multicast_MoveOnInput_Implementation */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MoveOnInput(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);
	void Multicast_MoveOnInput_Implementation(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);
	
	/** Server version of MoveOnInput
		Actual implementation is inside Server_MoveOnInput_Implementation */
	UFUNCTION(Server, Reliable)
	void Server_MoveOnInput(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);
	void Server_MoveOnInput_Implementation(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);

	/** Handles player camera movement */
	void RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);

	/** Multicast version of RotateCameraOnInput
		Actual implementation is inside Multicast_RotateCameraOnInput_Implementation */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);
	void Multicast_RotateCameraOnInput_Implementation(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);

	/** Server version of RotateCameraOnInput
		Actual implementation is inside Server_RotateCameraOnInput_Implementation */
	UFUNCTION(Server, Reliable)
	void Server_RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);
	void Server_RotateCameraOnInput_Implementation(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);

	/** (OVERRIDE) Called to set up replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	/** Changes value of forwardInput depending on player input */
	void MoveForward(float value);

	/** Changes value of rightInput depending on player input */
	void MoveRight(float value);

	/** Performs default playercharacter.Jump() among other calculations */
	void TryJumping();

	/** Update player isSprinting state on button press */
	void UpdateSprinting();

	// WINDOW FOCUS (FIXING ALT TAB BUG)
	/////////////////////////////////////////////////////////////////////

	/** NOT YET IMPLEMENTED */
	void OnFocusChanged(bool newFocusState);

	// CAMERA
	/////////////////////////////////////////////////////////////////////

	/** Changes cameraPitch variable depending on Mouse/Controller input */
	void LookUp(float value);

	/** Changes cameraYaw variable depending on Mouse/Controller input */
	void LookRight(float value);

	/** Updates different player states such as: isGrounded, isSprinting
		depending on current conditions*/
	void UpdateStates(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput);

	/** Multicast version of UpdateStates
		Actual implementation is inside Multicast_UpdateStates_Implementation */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateStates(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput);
	void Multicast_UpdateStates_Implementation(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput);

	/** Server version of UpdateStates
		Actual implementation is inside Server_UpdateStates_Implementation */
	UFUNCTION(Server, Reliable)
	void Server_UpdateStates(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput);
	void Server_UpdateStates_Implementation(Agame_PlayerCharacter* playerChar, bool is_Sprinting, float fInput);
	
	/** Stores move data while grounded for potential advanced air movement in the future */
	void StoreMoveDataWhileGrounded(Agame_PlayerCharacter* playerChar);

	/** Calculates currentFPS variable */
	void CalculateFPS(float DeltaTime);

	/** Updates speed depending on current sprint condition */
	void UpdateSpeed(Agame_PlayerCharacter* player, bool is_Sprinting, float fInput);

	/** Current FPS */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS")
	float currentFPS;

	/** Is true if player is sprinting */
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool isSprinting;


protected:

	// MOVEMENT
	/////////////////////////////////////////////////////////////////////

	/** Updated via keyboard/controller input */
	UPROPERTY(Replicated)
	float forwardInput;

	/** Updated via keyboard/controller input */
	UPROPERTY(Replicated)
	float rightInput;

	/** If input < threshold - ignore input */
	float inputThreshold;

	/** Updated via mouse/controller input */
	UPROPERTY(Replicated)
	float cameraPitch;

	/** Updated via mouse/controller input */
	UPROPERTY(Replicated)
	float cameraYaw;

	/** Current player movement direction */
	FVector movementDirection;

	/** Movement direction stored for advanced air control */
	FVector stored_movementDirection;
	
	/** Current max speed player can achieve, changes if he is sprinting */
	UPROPERTY(Replicated)
	float maxSpeed;

	/** Current player acceleration */
	float acceleration;

	/** Current player velocity */
	FVector currentVelocity;

	/** Velocity stored for advanced air control */
	FVector stored_currentVelocity;
	
	

	/** Player walking speed */
	float walkingSpeed;

	/** Player sprinting speed */
	float sprintingSpeed;

	//BETTER JUMP
	/////////////////////////////////////////////////////////////////////

	/** World time when player Jumped */
	float timeOfJumpPress;

	/** World time when player landed */
	float timeOfLanding;

	/** Used to improve jump. Time before landing, during which jump button press
		will trigger a jump even if pressed while not being grounded */
	float timeOfJumpBuffer;

	/** If player presses jump button in a certain time period
		before landing, jump will occur irrespective of 
		if the player is grounded or not */
	void ImproveJump(Agame_PlayerCharacter* playerChar);

	//TESTING
	/////////////////////////////////////////////////////////////////////

	/** allows to change framerate with a button press
		CTRL + I - set max FPS to 15
		CTRL + O - set max FPS to 30
		CTRL + K - set max FPS to 60
		CTRL + L - set max FPS to 144
		CTRL + M - set max FPS to UNLIMITED */
	void Enable_Framerate_Changer();

};
