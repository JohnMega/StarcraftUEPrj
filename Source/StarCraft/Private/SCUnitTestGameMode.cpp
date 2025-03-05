// Fill out your copyright notice in the Description page of Project Settings.


#include "SCUnitTestGameMode.h"
#include "Player/SC_UnitTestMainCamera.h"
#include "Player/SCPlayerController.h"

ASCUnitTestGameMode::ASCUnitTestGameMode()
{
	DefaultPawnClass = ASC_UnitTestMainCamera::StaticClass();
	PlayerControllerClass = ASCPlayerController::StaticClass();
}