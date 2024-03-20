// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SHOOTER_MP_API utility_General
{
public:
	/* Constructs utility_general class*/
	utility_General();

	/* Frees all the dynamically allocated memory */
	~utility_General();

	/* Finds actor by tag 
	   @param actors - array of actors to search in 
	   @param tag - look up actor that has this tag */
	static AActor* FindActorByTag(TArray<AActor*>& FoundActors, const FName& tag);

	/* Calculates angle between two vectors */
	static float Calculate_Angle(FVector& vector_1, FVector& vector_2);
};
