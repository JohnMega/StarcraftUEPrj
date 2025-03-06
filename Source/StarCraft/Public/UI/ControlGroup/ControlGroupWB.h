// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlGroupWB.generated.h"

class UImage;
class UTextBlock;
class ASCAICharacter;

UCLASS()
class STARCRAFT_API UControlGroupWB : public UUserWidget
{
	GENERATED_BODY()
	
	friend class UControlGroupsViewWB;

private:
	TArray<ASCAICharacter*> ControlGroupUnits;
	TArray<FDelegateHandle> ControlGroupUnitsOnDeathBinds;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* UnitImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UnitsCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BindButtonNumText;

private:
	void OnUnitDeath();
	void ClearAllOnCharacterDeathDelegate();

protected:
	void OnUpdateControlGroupInfo(const TArray<ASCAICharacter*>& NewControlGroupUnits, int32 BindButtonNum, int32 ControlGroupNum);
};
