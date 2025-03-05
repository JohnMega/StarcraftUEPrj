// Fill out your copyright notice in the Description page of Project Settings.


#include "SCGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SCPlayerController.h"
#include "Player/SC_MainCamera.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "OneToOne/OTOManager.h"

ASCGameModeBase::ASCGameModeBase()
{
	DefaultPawnClass = ASC_MainCamera::StaticClass();
	PlayerControllerClass = ASCPlayerController::StaticClass();
}