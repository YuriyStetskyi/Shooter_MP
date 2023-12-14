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