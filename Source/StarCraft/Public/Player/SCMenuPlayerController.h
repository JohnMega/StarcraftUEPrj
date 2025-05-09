// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCMenuPlayerController.generated.h"

UCLASS()
class STARCRAFT_API ASCMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<AActor> CurrentPawnOnSceneClass;

public:
	ASCMenuPlayerController();

protected:
	virtual void BeginPlay() override;
};
