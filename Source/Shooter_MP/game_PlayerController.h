// Fill out your copyright notice in the Description page of Project Settings.

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

	Agame_PlayerController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//movement
	virtual void SetupInputComponent() override;
	APawn* playerPawn;
	Agame_PlayerCharacter* playerCharacter;
	void MoveOnInput(Agame_PlayerCharacter* character, float fInput, float rInput, float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MoveOnInput(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);
	void Multicast_MoveOnInput_Implementation(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);
	UFUNCTION(Server, Reliable)
	void Server_MoveOnInput(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);
	void Server_MoveOnInput_Implementation(Agame_PlayerCharacter* playerChar, float fInput, float rInput, float DeltaTime);

	void RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);
	void Multicast_RotateCameraOnInput_Implementation(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);
	UFUNCTION(Server, Reliable)
	void Server_RotateCameraOnInput(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);
	void Server_RotateCameraOnInput_Implementation(Agame_PlayerCharacter* playerChar, float pitch, float yaw, float DeltaTime);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	void MoveForward(float value);
	void MoveRight(float value);
	void TryJumping();
	void UpdateSprinting();

	//window focus (fixing alt tab bug)
	void OnFocusChanged(bool newFocusState);

	//camera
	void LookUp(float value);
	void LookRight(float value);

	void UpdateStates(APawn* player);
	void StoreMoveDataWhileGrounded(Agame_PlayerCharacter* playerChar);

private:

	//movement
	UPROPERTY(Replicated)
	float forwardInput;
	UPROPERTY(Replicated)
	float rightInput;
	float inputThreshold;

	UPROPERTY(Replicated)
	float cameraPitch;
	UPROPERTY(Replicated)
	float cameraYaw;

	FVector movementDirection;
	FVector stored_movementDirection;
	
	float maxSpeed;
	float acceleration;
	FVector currentVelocity;
	FVector stored_currentVelocity;

	float walkingSpeed;
	float sprintingSpeed;

	//better jump
	float timeOfJumpPress;
	float timeOfLanding;
	float timeOfJumpBuffer;
	void ImproveJump(Agame_PlayerCharacter* playerChar);

private:
	//testing
	void Enable_Framerate_Changer();

};
