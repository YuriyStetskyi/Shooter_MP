// Fill out your copyright notice in the Description page of Project Settings.


#include "game_PlayerController.h"


Agame_PlayerController::Agame_PlayerController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void Agame_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void Agame_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}