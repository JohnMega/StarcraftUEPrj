// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OTORoundFinishWB.generated.h"

UCLASS()
class STARCRAFT_API UOTORoundFinishWB : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OTOGameEndingHandle();
};
