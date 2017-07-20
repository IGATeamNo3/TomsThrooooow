// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TomsThrooooowGameMode.h"
#include "TomsThrowLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOMSTHROOOOOW_API ATomsThrowLobbyGameMode : public ATomsThrooooowGameMode
{
	GENERATED_BODY()

		ATomsThrowLobbyGameMode();
public:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
};
