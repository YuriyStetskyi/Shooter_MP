// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "game_PlayerCharacter.generated.h"

UCLASS()
class SHOOTER_MP_API Agame_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Agame_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCapsuleComponent* capsuleCollider;
	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCameraComponent* camera;



private:


};
