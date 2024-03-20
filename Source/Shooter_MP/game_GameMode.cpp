// Copyright Epic Games, Inc. All Rights Reserved.


#include "game_GameMode.h"

Agame_GameMode::Agame_GameMode()
	:maxPlayers(4),
	currentPlayers(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerControllerClass = Agame_PlayerController::StaticClass();
	// Set the ticking group to PrePhysics so that this happens before all the other ticks
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	game_State = GetGameState<Agame_GameState>();
}



void Agame_GameMode::BeginPlay()
{

}

void Agame_GameMode::Tick(float DeltaTime)
{

}

void Agame_GameMode::PostLogin(APlayerController* NewPlayer)
{
	currentPlayers++;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Agame_PlayerCharacter::StaticClass(), playerCharacters);

	SetupMultiplayer(NewPlayer);
}

void Agame_GameMode::SetupMultiplayer(APlayerController* NewPlayer)
{
	switch (currentPlayers)
	{
	case(1):
	{
		AActor* toPosess = utility_General::FindActorByTag(playerCharacters,"Player1");
		NewPlayer->Possess((APawn*)toPosess);
		break;
	}
	case(2):
	{
		AActor* toPosess = utility_General::FindActorByTag(playerCharacters, "Player2");
		NewPlayer->Possess((APawn*)toPosess);
 		break;
	}
	case(3):
	{
		AActor* toPosess = utility_General::FindActorByTag(playerCharacters, "Player3");
		NewPlayer->Possess((APawn*)toPosess);
		break;
	}
	case(4):
	{
		AActor* toPosess = utility_General::FindActorByTag(playerCharacters, "Player4");
		NewPlayer->Possess((APawn*)toPosess);
		break;
	}
	default:
		break;
	}
}

void Agame_GameMode::SetupSingleplayer(APlayerController* NewPlayer)
{
	//TO IMPLEMENT
}
