// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitTestLevelPlayMenuWB.generated.h"

class UMediaPlayer;
class UMediaSource;
class UButton;

UCLASS()
class STARCRAFT_API UUnitTestLevelPlayMenuWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* CreateServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FindGameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaPlayer* UnitTestMediaPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaSource* UnitTestMediaSource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels Names")
	FName UnitTestLevelName = "Minimal_Default";

public:
	virtual bool Initialize() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
