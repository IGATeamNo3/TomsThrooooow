// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameLevelDataTable.generated.h"

USTRUCT(BlueprintType)
struct FGameLevelDataTable : public FTableRowBase
{
	GENERATED_BODY()

	FGameLevelDataTable()
		: BlinkCameraFadeColor(0.8f, 0.8f, 0.8f)
	{

	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName ActID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FText ActName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FText ActDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName LevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FText LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName PersistentLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> StreamingLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName NextLevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FText LevelDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TSubclassOf<UObject> LevelMission;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FString> LevelSpecifiedExecCommands;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TAssetPtr<class UFMODEvent> LevelDescriptionVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TAssetPtr<class UFMODEvent> ActDescriptionVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FLinearColor BlinkCameraFadeColor;
};
/**
 * 
 */
class BLACKSHIELDNETNATIVE_API GameLevelDataTable
{
public:
	GameLevelDataTable();
	~GameLevelDataTable();
};
