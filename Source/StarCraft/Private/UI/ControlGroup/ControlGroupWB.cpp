// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ControlGroup/ControlGroupWB.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AI/SCAICharacter.h"
#include "Components/HealthComponent.h"

void UControlGroupWB::OnUpdateControlGroupInfo(const TArray<ASCAICharacter*>& NewControlGroupUnits, int32 BindButtonNum, int32 ControlGroupNum)
{
	ClearAllOnCharacterDeathDelegate();

	ControlGroupUnits = NewControlGroupUnits;
	ControlGroupUnitsOnDeathBinds.Empty();
	for (int32 i = 0; i < ControlGroupUnits.Num(); ++i)
	{
		if (ControlGroupUnits[i])
		{
			ControlGroupUnitsOnDeathBinds.Add(ControlGroupUnits[i]->GetComponentByClass<UHealthComponent>()->OnCharacterDeath.AddUObject(this, &UControlGroupWB::OnUnitDeath));
		}
	}

	if (ControlGroupUnits.Num() > 0)
	{
		UnitImage->SetVisibility(ESlateVisibility::Visible);
		UnitImage->SetBrushFromTexture(ControlGroupUnits[0]->GetUnitControlGroupImage());
	}
	else
	{
		UnitImage->SetVisibility(ESlateVisibility::Hidden);
	}

	UnitsCountText->SetText(FText::FromString(ControlGroupNum == -1 ? " " : FString::FromInt(ControlGroupNum)));
	BindButtonNumText->SetText(FText::FromString(FString::FromInt(BindButtonNum)));
}

void UControlGroupWB::ClearAllOnCharacterDeathDelegate()
{
	for (int32 i = 0; i < ControlGroupUnits.Num(); ++i)
	{
		if (ControlGroupUnits[i])
		{
			auto HealthComponent = ControlGroupUnits[i]->GetComponentByClass<UHealthComponent>();
			if(HealthComponent) HealthComponent->OnCharacterDeath.Remove(ControlGroupUnitsOnDeathBinds[i]);
		}
	}
}

void UControlGroupWB::OnUnitDeath()
{
	int32 NewUnitCount = FCString::Atoi(*UnitsCountText->GetText().ToString()) - 1;
	if (NewUnitCount <= 0)
	{
		TArray<ASCAICharacter*> EmptyControlGroupUnits;
		OnUpdateControlGroupInfo(EmptyControlGroupUnits, FCString::Atoi(*BindButtonNumText->GetText().ToString()), -1);

		ClearAllOnCharacterDeathDelegate();
	}
	else
	{
		UnitsCountText->SetText(FText::FromString(FString::FromInt(NewUnitCount)));
	}
}