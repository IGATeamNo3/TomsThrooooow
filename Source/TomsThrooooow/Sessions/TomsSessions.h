// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySession, ECompeleteResult, Result);

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

	UPROPERTY(BlueprintAssignable, Category = Sessions)
		FOnDestroySession OnDestroyComplete;
public:



	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Sessions)
		void CreateSessionWithSetting(UObject* WorldContextObject, APlayerController* PC, bool bUseLAN);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Sessions)
		void FindSessionsWithSetting(UObject* WorldContextObject, APlayerController* PC, int32 MaxResults, bool bUseLAN);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Sessions)
		void JoinSessionWithSetting(UObject* WorldContextObject, APlayerController* PC, const FTomsBlueprintSessionResult& Session);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Sessions)
		void DestroyTomsSession(UObject* WorldContextObject, APlayerController* PC);
	

	UFUNCTION(BlueprintPure, Category = Sessions)
	static FString GetServerName(const FTomsBlueprintSessionResult& Result)
	{
		return Result.OnlineResult.Session.OwningUserName;
	}

	UFUNCTION(BlueprintPure, Category = Sessions)
	static int32 GetCurrentPlayers(const FTomsBlueprintSessionResult& Result)
	{
		return Result.OnlineResult.Session.SessionSettings.NumPublicConnections - Result.OnlineResult.Session.NumOpenPublicConnections;
	}

	UFUNCTION(BlueprintPure, Category = Sessions)
	static int32 GetMaxPlayers(const FTomsBlueprintSessionResult& Result)
	{
		return Result.OnlineResult.Session.SessionSettings.NumPublicConnections;
	}

	UFUNCTION(BlueprintPure, Category = Sessions)
	static int32 GetPingInMs(const FTomsBlueprintSessionResult& Result)
	{
		return Result.OnlineResult.PingInMs;
	}

	UFUNCTION(BlueprintPure, Category = Sessions)
	TArray<FString> GetPlayerNames();
	
	//UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Sessions)

public:
	
	void InitSessionSystem();
protected:
	TSharedPtr<const FUniqueNetId> GetPlayerUniqueID(APlayerController* PlayerController);
	UPROPERTY()
	UObject* World;
	UPROPERTY()
	APlayerController* PlayerController;
private:
	
	void OnCreateCompleted(FName SessionName, bool bSuccess);
	void OnStartCompleted(FName SessionName, bool bSuccess);
	void OnFindCompleted(bool bSuccess);
	void OnDestroyCompleted(FName SessionName, bool bWasSuccessful);
	void OnJoinCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FNamedOnlineSession* GetLocalSession() const;
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
