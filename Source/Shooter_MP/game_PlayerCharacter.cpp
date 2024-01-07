// Fill out your copyright notice in the Description page of Project Settings.


#include "game_PlayerCharacter.h"

// Sets default values
Agame_PlayerCharacter::Agame_PlayerCharacter()
	:isGrounded(false),
	isSprinting(false),
	fov_default(90.0f),
	fov_sprint_difference(20.0f),
	current_HitTime(0),
	lastHit_Time(0),
	isCornerStuck(false)
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

	capsuleCollider->OnComponentHit.AddDynamic(this, &Agame_PlayerCharacter::OnHit);
	//capsuleOverlapDetector->OnComponentBeginOverlap.AddDynamic(this, &Agame_PlayerCharacter::Detector_OnOverlapBegin);
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

void Agame_PlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	isHittingStuff = true;
	lastHit_Time = current_HitTime;
	current_HitTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	lastHit_ImpactNormal = currentHit_ImpactNormal;
	currentHit_ImpactNormal = Hit.ImpactNormal;

	//this is bad corner sticking fix for now, but fix it later or lose your mind trying to fix this for past 15 hours
	if (current_HitTime - lastHit_Time < 0.1 && currentHit_ImpactNormal != lastHit_ImpactNormal)
	{
		isCornerStuck = true;	

		hitNormal = (currentHit_ImpactNormal + lastHit_ImpactNormal) * 0.75;
		FString VectorString = FString::Printf(TEXT("CORNER HITTING - X: %f, Y: %f, Z: %f"), hitNormal.X, hitNormal.Y, hitNormal.Z);
		GEngine->AddOnScreenDebugMessage(-1, 1000.5f, FColor::Orange, VectorString, true, FVector2D(1.5, 1.5));
	}
	else
	{
		isCornerStuck = false;
		hitNormal = Hit.ImpactNormal;
		FString VectorString = FString::Printf(TEXT("HITTING - X: %f, Y: %f, Z: %f"), hitNormal.X, hitNormal.Y, hitNormal.Z);
		GEngine->AddOnScreenDebugMessage(-1, 1000.5f, FColor::Yellow, VectorString, true, FVector2D(1.5, 1.5));
	}


	////debugging
	//FString VectorString = FString::Printf(TEXT("HITTING - X: %f, Y: %f, Z: %f"), hitNormal.X, hitNormal.Y, hitNormal.Z);
	//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, VectorString, true, FVector2D(1.5, 1.5));	

	
}

void Agame_PlayerCharacter::Detector_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isHittingStuff = true;
	hitNormal = SweepResult.ImpactNormal;

	//debugging
	FString VectorString = FString::Printf(TEXT("OVERLAP - X: %f, Y: %f, Z: %f"), hitNormal.X, hitNormal.Y, hitNormal.Z);
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, VectorString, true, FVector2D(1.5, 1.5));

	//debugging
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("DETECTOR OVERLAPPING"), true, FVector2D(1.5, 1.5));
}

void Agame_PlayerCharacter::Detector_OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isHittingStuff = false;
	isCornerStuck = false;

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

