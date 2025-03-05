// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SCGameInstance.generated.h"

UCLASS()
class STARCRAFT_API USCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TMap<TSubclassOf<UUserWidget>, ESlateVisibility> SkipWidgets;

protected:
	virtual void Init() override;

	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	UFUNCTION(BlueprintImplementableEvent)
	void HandleNetworkFailure_Impl();

public:
	UFUNCTION(BlueprintCallable)
	void DeleteSomeWidgetsWhileNetworkFailure();
};
