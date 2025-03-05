// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OTOManager.generated.h"

enum class ETeamType : uint8;

UCLASS()
class STARCRAFT_API AOTOManager : public AActor
{
	GENERATED_BODY()

public:	
	AOTOManager();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OTOGameEndingHandle(ETeamType TeamType);

	bool IsOTOGameEnded();
};
