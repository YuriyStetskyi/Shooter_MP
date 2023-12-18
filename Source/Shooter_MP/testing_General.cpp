// Fill out your copyright notice in the Description page of Project Settings.


#include "testing_General.h"

testing_General::testing_General()
{
  
}

testing_General::~testing_General()
{

}

void testing_General::add_Framerate_Changer()
{
}

void testing_General::Set_FPS_15()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("FPS CHANGED TO 15"));
    GEngine->FixedFrameRate = 15.0;
    GEngine->bUseFixedFrameRate = true;
}

void testing_General::Set_FPS_30()
{
    GEngine->FixedFrameRate = 30.0;
    GEngine->bUseFixedFrameRate = true;
}

void testing_General::Set_FPS_60()
{
    GEngine->FixedFrameRate = 60.0;
    GEngine->bUseFixedFrameRate = true;
}

void testing_General::Set_FPS_144()
{
    GEngine->FixedFrameRate = 144.0;
    GEngine->bUseFixedFrameRate = true;
}

void testing_General::Set_FPS_uncap()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("FPS UNCAPPED"));
    GEngine->FixedFrameRate = 999.0;
    GEngine->bUseFixedFrameRate = false;
}
