// Fill out your copyright notice in the Description page of Project Settings.


#include "game_PlayerCharacter.h"

// Sets default values
Agame_PlayerCharacter::Agame_PlayerCharacter()
	:isGrounded(false),
	isSprinting(false),
	fov_default(90.0f),
	fov_sprint_difference(20.0f)
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
		camera->FieldOfView = fov_default;
		capsuleOverlapDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapDetector"));
		capsuleOverlapDetector->SetupAttachment(capsuleCollider);
		capsuleOverlapDetector->SetWorldLocation(FVector::ZeroVector);
		bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
		bodyMesh->SetupAttachment(capsuleCollider);
	}
}

// Called when the game starts or when spawned
void Agame_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	capsuleCollider->OnComponentBeginOverlap.AddDynamic(this, &Agame_PlayerCharacter::OnOverlapBegin);
	capsuleCollider->OnComponentEndOverlap.AddDynamic(this, &Agame_PlayerCharacter::OnOverlapEnd);
	capsuleCollider->OnComponentHit.AddDynamic(this, &Agame_PlayerCharacter::OnHit);
	capsuleOverlapDetector->OnComponentEndOverlap.AddDynamic(this, &Agame_PlayerCharacter::Detector_OnOverlapEnd);
}

// Called every frame
void Agame_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ChangeFovWhenSprinting();
}

// Called to bind functionality to input
void Agame_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void Agame_PlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isOverlappingStuff = true;

	//debugging
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("OVERLAPPING"), true, FVector2D(2, 2));
	
}

void Agame_PlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isOverlappingStuff = false;

	//debugging
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("NOT OVERLAPPING"), true, FVector2D(2, 2));
}

void Agame_PlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	isHittingStuff = true;
	//if (FMath::Abs(Hit.ImpactNormal.Z) < 0.1)
	{
		overlapNormal = Hit.ImpactNormal;

		//debugging
		FString VectorString = FString::Printf(TEXT("HITTING - X: %f, Y: %f, Z: %f"), overlapNormal.X, overlapNormal.Y, overlapNormal.Z);
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, VectorString, true, FVector2D(1.5, 1.5));
	}
	
	
}

void Agame_PlayerCharacter::Detector_OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isHittingStuff = false;

	//debugging
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("DETECTOR NOT OVERLAPPING"), true, FVector2D(1.5, 1.5));
}


void Agame_PlayerCharacter::ChangeFovWhenSprinting()
{
	float easingSpeed = 0.1;
	if (isSprinting)
	{
		camera->FieldOfView = FMath::Lerp(camera->FieldOfView, fov_default + fov_sprint_difference, easingSpeed);
	}
	else
	{
		camera->FieldOfView = FMath::Lerp(camera->FieldOfView, fov_default, easingSpeed);
	}
}

