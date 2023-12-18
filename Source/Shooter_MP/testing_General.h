// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SHOOTER_MP_API testing_General
{
public:
	testing_General();
	~testing_General();

	void add_Framerate_Changer();

	void Set_FPS_15();
	void Set_FPS_30();
	void Set_FPS_60();
	void Set_FPS_144();
	void Set_FPS_uncap();

private:
};
