// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "utility_General.h"
#include "Kismet/GameplayStatics.h"
#include "game_PlayerCharacter.h"
#include "game_PlayerController.h"
#include "game_GameState.h"
#include "game_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_MP_API Agame_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	//OVERRIDES (WERE HERE AT CLASS CREATION)
	/////////////////////////////////////////////////////////////////////

	/* Sets default values for this character's properties*/
	Agame_GameMode();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Called after a player logs into a level */
	void PostLogin(APlayerController* NewPlayer) override;

	// LOGIN
	/////////////////////////////////////////////////////////////////////

	/* Looks for a not posessed actor in playerCharacters and posesses if possible */
	void SetupMultiplayer(APlayerController* NewPlayer);

	/* NOT YET IMPLEMENTED */
	void SetupSingleplayer(APlayerController* NewPlayer);
	
	/* Array that contains all the actors to be posessed 
	   or already posessed by playerController's */
	TArray<AActor*> playerCharacters;

	/* Actor that is possessed by the FIRST playerController that logged in */
	AActor* player_1;
	/* Actor that is possessed by the SECOND playerController that logged in */
	AActor* player_2;
	/* Actor that is possessed by the THIRD playerController that logged in */
	AActor* player_3;
	/* Actor that is possessed by the FOURTH playerController that logged in */
	AActor* player_4;

	/* Maximum amount of players that can join the game */
	int maxPlayers;

	/* Amount of players currently logged into a map */
	int currentPlayers;

	/* Contains game state information */
	Agame_GameState* game_State;

private:



};
