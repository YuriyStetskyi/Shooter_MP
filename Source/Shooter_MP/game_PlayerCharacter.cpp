// Copyright Epic Games, Inc. All Rights Reserved.


#include "game_PlayerCharacter.h"

Agame_PlayerCharacter::Agame_PlayerCharacter()
	:isGrounded(false),
	isSprinting(false),
	fov_default(90.0f),
	fov_sprint_difference(20.0f),
	current_HitTime(0),
	lastHit_Time(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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
		//bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
		bodyMesh = FindComponentByClass<USkeletalMeshComponent>();
		bodyMesh->SetupAttachment(capsuleCollider);
		gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMesh"));
		gunMesh->SetupAttachment(camera);
	}
	NetUpdateFrequency = 5000.0f;
}

// Called when the game starts or when spawned
void Agame_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


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

void Agame_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(Agame_PlayerCharacter, camera);
	DOREPLIFETIME(Agame_PlayerCharacter, bodyMesh);
	DOREPLIFETIME(Agame_PlayerCharacter, gunMesh);
}

void Agame_PlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	isHittingStuff = true;
	lastHit_Time = current_HitTime;
	current_HitTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	lastHit_ImpactNormal = currentHit_ImpactNormal;
	currentHit_ImpactNormal = Hit.ImpactNormal;
	float timeBetweenFrames = GetWorld()->DeltaTimeSeconds;

	//this is bad corner sticking fix for now, but fix it later or lose your mind trying to fix this for past 15 hours
	if (current_HitTime - lastHit_Time < timeBetweenFrames * 10 && utility_General::Calculate_Angle(currentHit_ImpactNormal, lastHit_ImpactNormal) > 30)
	{	
		hitNormal = (currentHit_ImpactNormal + lastHit_ImpactNormal) * 0.75;
	}
	else
	{
		hitNormal = Hit.ImpactNormal;
	}
}

void Agame_PlayerCharacter::Detector_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//inactive right now - look OnComponentBeginOverlap.AddDynamic
	isHittingStuff = true;
	
}

void Agame_PlayerCharacter::Detector_OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isHittingStuff = false;
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

