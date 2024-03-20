// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "utility_General.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "game_PlayerCharacter.generated.h"

UCLASS()
class SHOOTER_MP_API Agame_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// OVERRIDES (WERE HERE AT CLASS CREATION)
	/////////////////////////////////////////////////////////////////////

	/* Sets default values for this character's properties */
	Agame_PlayerCharacter();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/*  Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** (OVERRIDE) Called to set up replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// COMPONENTS
	/////////////////////////////////////////////////////////////////////

	/* Player collider that is used to determine if player is hitting objects */
	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCapsuleComponent* capsuleCollider;
	
	/* Player camera */
	UPROPERTY(Replicated, EdiTAnywhere, Category = "cpp_MyStuff")
	UCameraComponent* camera;
	
	/* Player collider that is used to determine if player is overlapping with objects */
	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCapsuleComponent* capsuleOverlapDetector;
	
	/* Main player mesh */
	UPROPERTY(Replicated, EditAnywhere, Category = "cpp_MyStuff")
	USkeletalMeshComponent* bodyMesh;

	/** Main gun mesh*/
	UPROPERTY(Replicated, EditAnywhere, Category = "cpp_MyStuff")
	USkeletalMeshComponent* gunMesh;

	// STATES
	/////////////////////////////////////////////////////////////////////

	/* Is true if player is currently touching the ground */
	UPROPERTY(VisibleAnywhere, Category = "cpp_States")
	bool isGrounded;
	
	/* Is true if player is currently sprinting */
	UPROPERTY(VisibleAnywhere, Category = "cpp_States")
	bool isSprinting;

	// CAMERA
	/////////////////////////////////////////////////////////////////////
	
	/* Default player camera Field Of View */
	float fov_default;

	/* Amount by which Field Of View should increase when sprinting */
	float fov_sprint_difference;

	// COLLISION
	/////////////////////////////////////////////////////////////////////
	
	/* Normal vector of the plane that is being hit by capsuleCollider */
	FVector hitNormal;

	/* Is true if capsuleOverlapDetector is overlapping with walls that player should slide off */
	bool isOverlappingStuff;

	/* (OVERLOADED) This method can be subscribed to OnComponentHit event
	   Calculates needed information to successfully collide with stuff
	   @param NormalImpulse - impulse that is being applied(if it is) along
	   the normal vector of the collision */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	/* Can be subscribed to OnComponentOverlap event
	   Handles all required information when component 
	   starts overlapping with some other actor */
	UFUNCTION()
	void Detector_OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Can be subscribed to OnComponentOverlap event
	   Handles all required information when component 
	   stops overlapping with some other actor */
	UFUNCTION()
	void Detector_OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* Is true if capsuleCollider is hitting some other actor */
	bool isHittingStuff;

	/* System time in seconds when the hit occured */
	float current_HitTime;

	/* System time in seconds when previous hit occured */
	float lastHit_Time;

	/* Vector representing the Normal vector of plane collided with */
	FVector currentHit_ImpactNormal;

	/* Vector representing the Normal vector of plane collided with last time */
	FVector lastHit_ImpactNormal;

private:

	/* Handles camera Field Of View changes when sprinting */
	void ChangeFovWhenSprinting();
};
