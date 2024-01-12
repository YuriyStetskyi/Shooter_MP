// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "utility_General.h"
#include "Kismet/GameplayStatics.h"
#include "game_PlayerCharacter.h"
#include "game_PlayerController.h"
#include "game_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_MP_API Agame_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	//overrides
	Agame_GameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void PostLogin(APlayerController* NewPlayer) override;

	//login
	void SetupMultiplayer(APlayerController* NewPlayer);
	void SetupSingleplayer(APlayerController* NewPlayer);


	//login
	TArray<AActor*> playerCharacters;
	AActor* player_1;
	AActor* player_2;
	AActor* player_3;
	AActor* player_4;
	int maxPlayers;
	int currentPlayers;

private:

	//void FindPlayerCharacters();

};
