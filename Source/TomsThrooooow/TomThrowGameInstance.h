// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "DataTables/GameLevelDataTable.h"
#include "TomThrowGameInstance.generated.h"

/**
 * 
 */


UCLASS()
class TOMSTHROOOOOW_API UTomThrowGameInstance : public UGameInstance
{
	GENERATED_BODY()

	

public:

	UTomThrowGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init() override;

	UFUNCTION(BlueprintPure, Category = Sessions)
		UTomsSessions* GetTomsSession() const { return TomsSession; }

	UFUNCTION(BlueprintPure, Category = GameLevelTable)
		bool GetGameLevelData(FName RowName, FGameLevelDataTable& OutRow);

	UFUNCTION(BlueprintPure, Category = GameLevelTable)
		TArray<FName> GetGameLevelRowNames() const { return GameLevelRowNames; }


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		UDataTable* GameLevelDataTable;


protected:
	UPROPERTY()
	class UTomsSessions* TomsSession;


private:
	TArray<FGameLevelDataTable*>                        GameLevelDatas;
	TArray<FName>										GameLevelRowNames;
};

