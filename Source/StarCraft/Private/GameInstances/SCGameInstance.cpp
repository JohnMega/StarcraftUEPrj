// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/SCGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameMapsSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FunctionLibraries/SCFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "OneToOne/OTOManager.h"
#include "UI/GameOver/GameOverWB.h"

void USCGameInstance::Init()
{
	Super::Init();

	GetEngine()->OnNetworkFailure().Clear();
	GetEngine()->OnNetworkFailure().AddUObject(this, &USCGameInstance::HandleNetworkFailure);
}

void USCGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	auto OTORoundEndScreenWB = USCFunctionLibrary::GetWidgetByClass<UGameOverWB>(GetWorld());
	if (OTORoundEndScreenWB && OTORoundEndScreenWB->GetVisibility() == ESlateVisibility::Hidden)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		HandleNetworkFailure_Impl();
	}
}

void USCGameInstance::DeleteSomeWidgetsWhileNetworkFailure()
{
	TArray<UUserWidget*> AllWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, UUserWidget::StaticClass(), false);
	for (int32 i = 0; i < AllWidgets.Num(); ++i)
	{
		bool IsNeedToDelete = true;
		for (int32 j = 0; j < SkipWidgets.Num(); ++j)
		{
			auto SkipWidget = USCFunctionLibrary::GetWidgetByUClass(GetWorld(), SkipWidgets.Get(FSetElementId::FromInteger(j)).Key);
			if (SkipWidget == AllWidgets[i])
			{
				AllWidgets[i]->SetVisibility(SkipWidgets.Get(FSetElementId::FromInteger(j)).Value);
				IsNeedToDelete = false;
				break;
			}
		}

		if (IsNeedToDelete) AllWidgets[i]->RemoveFromParent();
	}
}