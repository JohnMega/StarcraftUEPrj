// Fill out your copyright notice in the Description page of Project Settings.


#include "SCGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SCPlayerController.h"
#include "Player/SC_MainCamera.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "UI/OneToOne/RoundFinish/OTORoundFinishWB.h"

ASCGameModeBase::ASCGameModeBase()
{
	DefaultPawnClass = ASC_MainCamera::StaticClass();
	PlayerControllerClass = ASCPlayerController::StaticClass();
}

void ASCGameModeBase::Logout(AController* Exiting)
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Logout");
	Super::Logout(Exiting);

	USCFunctionLibrary::GetWidgetByClass<UOTORoundFinishWB>(GetWorld())->OTOGameEndingHandle();
}