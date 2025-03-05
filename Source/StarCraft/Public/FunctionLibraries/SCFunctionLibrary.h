// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SCFunctionLibrary.generated.h"

UCLASS()
class STARCRAFT_API USCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	template<typename T>
	static T* GetActorByID(int32 ActorID, UWorld* World)
	{
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(World, T::StaticClass(), AllActors);
		for (int32 i = 0; i < AllActors.Num(); ++i)
		{
			if (ActorID == AllActors[i]->GetUniqueID())
			{
				return Cast<T>(AllActors[i]);
			}
		}

		return nullptr;
	}

	template<typename T>
	static T* GetWidgetByClass(UWorld* World)
	{
		TArray<UUserWidget*> AllWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, AllWidgets, T::StaticClass(), false);
		return AllWidgets[0] ? Cast<T>(AllWidgets[0]) : nullptr;
	}
};
