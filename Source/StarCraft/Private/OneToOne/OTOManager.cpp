// Fill out your copyright notice in the Description page of Project Settings.


#include "OneToOne/OTOManager.h"
#include "OneToOne/WavesNPCSpawner.h"
#include "UI/OneToOne/Score/OTOScoreWB.h"
#include "FunctionLibraries/SCFunctionLibrary.h"

AOTOManager::AOTOManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AOTOManager::IsOTOGameEnded()
{
	auto OTOScoreWB = USCFunctionLibrary::GetWidgetByClass<UOTOScoreWB>(GetWorld());
	if (OTOScoreWB)
	{
		return OTOScoreWB->IsOTOGameEnded();
	}
	else
	{
		return false;
	}
}