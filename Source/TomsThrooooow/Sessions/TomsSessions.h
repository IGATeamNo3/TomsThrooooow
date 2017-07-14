// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "OnlineSessionInterface.h"
#include "TomsSessions.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECompeleteResult : uint8
{
	EC_Success,
	EC_Failure
};

USTRUCT(BlueprintType)
struct FTomsBlueprintSessionResult
{
	GENERATED_USTRUCT_BODY()
public:
	FOnlineSessionSearchResult OnlineResult;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSession, ECompeleteResult, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSession, ECompeleteResult, Result, const TArray<FTomsBlueprintSessionResult>&, Sesssions);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinSession, ECompeleteResult, Result);

UCLASS(BlueprintType)
class TOMSTHROOOOOW_API UTomsSessions : public UObject
{
	GENERATED_BODY()
		UTomsSessions();
	~UTomsSessions();
public:
	UPROPERTY(BlueprintAssignable, Category = Sessions)
		FOnCreateSession OnCreateComplete;

	UPROPERTY(BlueprintAssignable, Category = Sessions)
		FOnFindSession OnFindComplete;

	UPROPERTY(BlueprintAssignable, Category = Sessions)
		FOnJoinSession OnJoinComplete;
public:



	UFUNCTION(BlueprintCallable, Category = Sessions)
		void CreateSessionWithSetting(APlayerController* PC, bool bUseLAN);

	UFUNCTION(BlueprintCallable, Category = Sessions)
		void FindSessionsWithSetting(APlayerController* PC, int32 MaxResults, bool bUseLAN);

	UFUNCTION(BlueprintCallable, Category = Sessions)
		void JoinSessionWithSetting(APlayerController* PC, const FTomsBlueprintSessionResult& Session);

	UFUNCTION(BlueprintPure, Category = Sessions)
	static FString GetServerName(const FTomsBlueprintSessionResult& Result)
	{
		return Result.OnlineResult.Session.OwningUserName;
	}
	
public:
	
	void InitSessionSystem();
protected:
	TSharedPtr<const FUniqueNetId> GetPlayerUniqueID(APlayerController* PlayerController);
private:
	
	void OnCreateCompleted(FName SessionName, bool bSuccess);
	void OnStartCompleted(FName SessionName, bool bSuccess);
	void OnFindCompleted(bool bSuccess);
	void OnDestroyCompleted(FName SessionName, bool bWasSuccessful);
	void OnJoinCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
private:
	
	FOnCreateSessionCompleteDelegate CreateCompleteDelegate;

	FOnStartSessionCompleteDelegate StartCompleteDelegate;

	FOnFindSessionsCompleteDelegate FindCompleteDelegate;

	FOnJoinSessionCompleteDelegate JoinCompleteDelegate;

	FOnDestroySessionCompleteDelegate DestroyCompleteDelegate;
	
	FDelegateHandle CreateCompleteDelegateHandle;
	FDelegateHandle StartCompleteDelegateHandle;
	FDelegateHandle FindCompleteDelegateHandle;
	FDelegateHandle JoinCompleteDelegateHandle;
	FDelegateHandle DestroyCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSearch> SearchObject;
	
	
};
