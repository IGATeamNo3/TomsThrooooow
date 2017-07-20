// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TomThrowBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TOMSTHROOOOOW_API UTomThrowBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "TomThrow | Library")
		static void TravelLevel(UObject* WorldContextObject, FName LevelName, TSubclassOf<AGameMode> GameModeClass, FName SubLevelName, const FString& Opition);

	UFUNCTION(BlueprintCallable, Category = "TomThrow | Library")
		static bool IsDevelopMode();
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "TomThrow | Library")
		static class UTomsSessions* GetTomsSession(UObject* WorldContextObject);

	
};
