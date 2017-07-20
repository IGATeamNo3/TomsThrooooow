// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "TomThrowGameInstance.h"
#include "TomThrowBlueprintFunctionLibrary.h"




void UTomThrowBlueprintFunctionLibrary::TravelLevel(UObject* WorldContextObject, FName LevelName, TSubclassOf<AGameMode> GameModeClass, FName SubLevelName, const FString& Opition)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (!World || LevelName == NAME_None)
	{
		return;
	}
	FURL URL;
	URL.Map = LevelName.ToString();
	FString GameModePath;
	if (GameModeClass)
	{
		GameModePath = GameModeClass->GetPathName();
		URL.Op.Add(TEXT("game=") + GameModePath);
		UE_LOG(LogTemp, Log, TEXT("GameModePath : %s"), *GameModePath)

	}
	if (SubLevelName != NAME_None)
	{
		URL.Op.Add(TEXT("sublevel=") + SubLevelName.ToString());
	}
	URL.Op.Add(Opition);
	if (IsDevelopMode())
	{
		GEngine->SetClientTravel(World, *URL.ToString(), ETravelType::TRAVEL_Absolute);
	}
	else
	{
		World->ServerTravel(URL.ToString());
	}
	UE_LOG(LogTemp, Log, TEXT("URL : %s"), *URL.ToString())
}

bool UTomThrowBlueprintFunctionLibrary::IsDevelopMode()
{
	return !(IsRunningGame() || IsRunningDedicatedServer());
}

UTomsSessions* UTomThrowBlueprintFunctionLibrary::GetTomsSession(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		UTomThrowGameInstance* GameInstance = Cast<UTomThrowGameInstance>(World->GetGameInstance());
		if (GameInstance)
		{
			return GameInstance->GetTomsSession();
		}
	}
	
	return nullptr;

}
