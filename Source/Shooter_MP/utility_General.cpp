// Fill out your copyright notice in the Description page of Project Settings.


#include "utility_General.h"

utility_General::utility_General()
{
}

utility_General::~utility_General()
{
}


AActor* utility_General::FindActorByTag(TArray<AActor*>& actors, const FName& tag)
{
	for (AActor* actor : actors)
	{
		if (actor && actor->ActorHasTag(tag))
		{
			return actor;
		}
	}
	return nullptr;
}

float utility_General::Calculate_Angle(FVector& vector_1, FVector& vector_2)
{
	FVector normalized_V1 = vector_1.GetSafeNormal();
	FVector normalized_V2 = vector_2.GetSafeNormal();
	float Dot = FVector::DotProduct(normalized_V1, normalized_V2);
	float Angle_Radians = FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f));
	float Angle_Degrees = FMath::RadiansToDegrees(Angle_Radians);

	return Angle_Degrees;
}