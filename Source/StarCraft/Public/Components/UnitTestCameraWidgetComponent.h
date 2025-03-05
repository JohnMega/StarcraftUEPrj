// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MainCameraWidgetComponent.h"
#include "UnitTestCameraWidgetComponent.generated.h"

UCLASS()
class STARCRAFT_API UUnitTestCameraWidgetComponent : public UMainCameraWidgetComponent
{
	GENERATED_BODY()
	
protected:
	UInGameMenuWB* InGameMenuWidget;
	class UOTOPlayersAcceptWB* PlayersAcceptWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UInGameMenuWB> InGameMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UOTOPlayersAcceptWB> PlayersAcceptClass;

protected:
	virtual void BeginPlay() override;
};
