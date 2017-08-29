// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Sessions/TomsSessions.h"
#include "TomThrowGameInstance.h"



UTomThrowGameInstance::UTomThrowGameInstance(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	GameLevelDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("DataTable'/Game/TomsThrooooow/Blueprints/Mis/DataTable/DT_GameLevel.DT_GameLevel'")));

}
void UTomThrowGameInstance::Init()
{
	Super::Init();
	if (GameLevelDataTable)
	{
		GameLevelRowNames = GameLevelDataTable->GetRowNames();
		GameLevelDataTable->GetAllRows<FGameLevelDataTable>(FString(), GameLevelDatas);
	}
	TomsSession = NewObject<UTomsSessions>();
	TomsSession->InitSessionSystem();
	//InitSessionSystem();
}

bool UTomThrowGameInstance::GetGameLevelData(FName RowName, FGameLevelDataTable& OutRow)
{
	if (GameLevelDataTable)
	{
		FGameLevelDataTable *RowInfo = GameLevelDataTable->FindRow<FGameLevelDataTable>(RowName, FString());
		if (RowInfo)
		{
			OutRow = *RowInfo;
			return true;
		}
	}
	return false;
}

