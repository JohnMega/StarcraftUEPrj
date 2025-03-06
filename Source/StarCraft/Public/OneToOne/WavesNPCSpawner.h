// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WavesNPCSpawner.generated.h"

class AFriendUnitsSpawnPoint;
class AEnemyUnitsSpawnPoint;
class ASCMarine;
class ASCMinion;
class ASCAICharacter;
class ASCAIController;

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	BLUE_TEAM_TYPE = 0,
	RED_TEAM_TYPE
};

USTRUCT(BlueprintType)
struct FNPCWave
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<ASCAICharacter>> NPCClasses; // must be the same size

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<int32> NPCCounts; // must be the same size
};

UCLASS()
class STARCRAFT_API AWavesNPCSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SpawnPoint Classes")
	TSubclassOf<AFriendUnitsSpawnPoint> FriendUnitsSpawnPointClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnPoint Classes")
	TSubclassOf<AEnemyUnitsSpawnPoint> EnemyUnitsSpawnPointClass;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Classes")
	TSubclassOf<ASCMarine> MarineClass;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Classes")
	TSubclassOf<ASCMinion> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Controller Classes")
	TSubclassOf<ASCAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Waves")
	TArray<FNPCWave> NPCWaves;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teams")
	UMaterialInterface* RedTeamOverlayColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teams")
	UMaterialInterface* BlueTeamOverlayColor;

	int32 CurrentWave;

private:
	void UnitSpawn(UClass* UnitClass, ETeamType CurrTeamType);

public:	
	AWavesNPCSpawner();

	UFUNCTION(BlueprintCallable)
	void SpawnNPCWaves(int32 Wave);

	void SetTeamOverlayMaterial(ASCAICharacter* SCUnit, ETeamType CurrTeamType);
	void UnitSpawn_Impl(UClass* UnitClass, ETeamType CurrTeamType);
	int32 GetWavesCount() const { return NPCWaves.Num(); }
	int32 GetCurrentWave() { return CurrentWave; }
};
