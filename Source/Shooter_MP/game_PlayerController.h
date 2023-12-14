// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

private:
	
};
