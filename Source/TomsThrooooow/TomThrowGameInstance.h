// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "TomThrowGameInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECompeleteResult : uint8
{
	EC_Success,
	EC_Failure
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSession, ECompeleteResult, Result);

UCLASS()
class TOMSTHROOOOOW_API UTomThrowGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Sessions)
		FOnCreateSession OnCreateComplete;
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
	void OnCreateCompleted(FName SessionName, bool bSuccess);
	void OnStartCompleted(FName SessionName, bool bSuccess);
private:
	// The delegate executed by the online subsystem
	FOnCreateSessionCompleteDelegate CreateCompleteDelegate;

	// The delegate executed by the online subsystem
	FOnStartSessionCompleteDelegate StartCompleteDelegate;

	FDelegateHandle CreateCompleteDelegateHandle;
	FDelegateHandle StartCompleteDelegateHandle;
};

