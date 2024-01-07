// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Containers/Array.h"
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

	//components
	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCapsuleComponent* capsuleCollider;
	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCameraComponent* camera;
	UPROPERTY(EdiTAnywhere, Category = "cpp_MyStuff")
	UCapsuleComponent* capsuleOverlapDetector;
	UPROPERTY(EditAnywhere, Category = "cpp_MyStuff")
	UStaticMeshComponent* bodyMesh;

	//states
	UPROPERTY(VisibleAnywhere, Category = "cpp_States")
	bool isGrounded;
	UPROPERTY(VisibleAnywhere, Category = "cpp_States")
	bool isSprinting;

	//camera
	float fov_default;
	float fov_sprint_difference;

	//collision
	FVector hitNormal;
	bool isOverlappingStuff;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Detector_OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Detector_OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	bool isHittingStuff;

	float current_HitTime;
	float lastHit_Time;
	FVector currentHit_ImpactNormal;
	FVector lastHit_ImpactNormal;
	UPROPERTY(VisibleAnywhere, Category = "cpp_States")
	bool isCornerStuck;

private:

	void ChangeFovWhenSprinting();


};
