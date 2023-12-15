// Fill out your copyright notice in the Description page of Project Settings.


#include "game_PlayerCharacter.h"

// Sets default values
Agame_PlayerCharacter::Agame_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//components
	capsuleCollider = FindComponentByClass<UCapsuleComponent>();
	if (capsuleCollider != nullptr)
	{
		SetRootComponent(capsuleCollider);
		camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		camera->SetupAttachment(capsuleCollider);
	}
	
}

// Called when the game starts or when spawned
void Agame_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Agame_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Agame_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

