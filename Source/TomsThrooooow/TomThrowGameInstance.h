// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "TomThrowGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOMSTHROOOOOW_API UTomThrowGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init()override;
	
	UFUNCTION(BlueprintCallable, Category = Sessions)
	void CreateSessionWithSetting(APlayerController* PC, bool bUseLAN);

	UFUNCTION(BlueprintCallable, Category = Sessions)
	void FindSessionsWithSetting(APlayerController* PC, int32 MaxResults, bool bUseLAN);
	
	//UFUNCTION(BlueprintPure, Category = Sessions)
protected:
	TSharedPtr<const FUniqueNetId> GetPlayerUniqueID(APlayerController* PlayerController);

	void HandleCreateSessionComplete(FName SessionName, bool bSuccess);

	void HandleFindSessionsComplete(bool bSuccess);

	void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType);

private:
	void InitSessionSystem();
};
