// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SHOOTER_MP_API utility_General
{
public:
	utility_General();
	~utility_General();

	static AActor* FindActorByTag(TArray<AActor*>& FoundActors, const FName& tag);
};
