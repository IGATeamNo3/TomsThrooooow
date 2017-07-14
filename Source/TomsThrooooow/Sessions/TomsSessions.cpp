// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "TomsSessions.h"


const FName SessionName(TEXT("Game"));


UTomsSessions::UTomsSessions()
{

}

UTomsSessions::~UTomsSessions()
{

}

void UTomsSessions::CreateSessionWithSetting(APlayerController* PC, bool bUseLAN)
{
	//IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld());
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();

	if (Sessions)
	{
		FOnlineSessionSettings Settings;

		Settings.NumPublicConnections = 10;
		Settings.bShouldAdvertise = true;
		Settings.bAllowJoinInProgress = true;
		Settings.bUsesPresence = true;
		Settings.bAllowJoinViaPresence = true;
		Settings.bIsLANMatch = bUseLAN;
		

		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);

		CreateCompleteDelegateHandle = Sessions->GetSessionInterface()->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);
		Sessions->GetSessionInterface()->CreateSession(*UserID, SessionName, Settings);
	}
}

void UTomsSessions::FindSessionsWithSetting(APlayerController* PC, int32 MaxResults, bool bUseLAN)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();
	if (Sessions)
	{
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);

		SearchObject = MakeShareable(new FOnlineSessionSearch);
		if (SearchObject.IsValid())
		{
			SearchObject->MaxSearchResults = MaxResults;
			SearchObject->bIsLanQuery = bUseLAN;
			SearchObject->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			FindCompleteDelegateHandle = Sessions->GetSessionInterface()->AddOnFindSessionsCompleteDelegate_Handle(FindCompleteDelegate);
			Sessions->GetSessionInterface()->FindSessions(*UserID, SearchObject.ToSharedRef());
		}	
	}

}

void UTomsSessions::JoinSessionWithSetting(APlayerController* PC, const FTomsBlueprintSessionResult& Session)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();
	if (Sessions)
	{
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);
		JoinCompleteDelegateHandle = Sessions->GetSessionInterface()->AddOnJoinSessionCompleteDelegate_Handle(JoinCompleteDelegate);
		Sessions->GetSessionInterface()->JoinSession(*UserID, SessionName, Session.OnlineResult);
	}
}

TSharedPtr<const FUniqueNetId> UTomsSessions::GetPlayerUniqueID(APlayerController* PlayerController)
{
	TSharedPtr<const FUniqueNetId> UserID;

	if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
	{
		UserID = PlayerState->UniqueId.GetUniqueNetId();
		if (!UserID.IsValid())
		{
			UE_LOG(LogTomThrow, Warning, TEXT("Cannot map local player to unique net ID"))
				//FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Cannot map local player to unique net ID"), FunctionContext), ELogVerbosity::Warning);
		}
	}
	else
	{
		UE_LOG(LogTomThrow, Warning, TEXT("Invalid player state"))
			//FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid player state"), FunctionContext), ELogVerbosity::Warning);
	}
	return UserID;
}


void UTomsSessions::InitSessionSystem()
{
	CreateCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted);
	StartCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted);
	FindCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindCompleted);
	DestroyCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroyCompleted);
	JoinCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinCompleted);
}

void UTomsSessions::OnCreateCompleted(FName InSessionName, bool bSuccess)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();

	if (Sessions)
	{
		Sessions->GetSessionInterface()->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);
		if (bSuccess)
		{
			StartCompleteDelegateHandle = Sessions->GetSessionInterface()->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
			Sessions->GetSessionInterface()->StartSession(InSessionName);
			return;
		}
	}
	if (!bSuccess)
	{
		OnCreateComplete.Broadcast(ECompeleteResult::EC_Failure);
		UE_LOG(LogTomThrow, Error, TEXT("CreateSessionFaild"));
	}
}

void UTomsSessions::OnStartCompleted(FName InSessionName, bool bSuccess)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();

	if (Sessions)
	{
		Sessions->GetSessionInterface()->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
		if (bSuccess)
		{
			OnCreateComplete.Broadcast(ECompeleteResult::EC_Success);
			return;
		}
	}

	if (!bSuccess)
	{
		OnCreateComplete.Broadcast(ECompeleteResult::EC_Failure);
	}

}

void UTomsSessions::OnFindCompleted(bool bSuccess)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();
	if (Sessions)
	{
		Sessions->GetSessionInterface()->ClearOnFindSessionsCompleteDelegate_Handle(FindCompleteDelegateHandle);
	}

	TArray<FTomsBlueprintSessionResult> Results;

	if (bSuccess && SearchObject.IsValid())
	{
		for (auto& Result : SearchObject->SearchResults)
		{
			FTomsBlueprintSessionResult BPResult;
			BPResult.OnlineResult = Result;
			Results.Add(BPResult);
		}

		OnFindComplete.Broadcast(ECompeleteResult::EC_Success, Results);
	}
	else
	{
		OnFindComplete.Broadcast(ECompeleteResult::EC_Failure, Results);
	}
}
void UTomsSessions::OnJoinCompleted(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();
	if (Sessions)
	{
		Sessions->GetSessionInterface()->ClearOnJoinSessionCompleteDelegate_Handle(JoinCompleteDelegateHandle);
		FString ConnectString;
		if (Sessions->GetSessionInterface()->GetResolvedConnectString(SessionName, ConnectString))
		{
			UE_LOG(LogTomThrow, Log, TEXT("Join Session: Traveling to %s"), *ConnectString)
			OnJoinComplete.Broadcast(ECompeleteResult::EC_Success);
			return;
		}
	}
	OnJoinComplete.Broadcast(ECompeleteResult::EC_Failure);

}

void UTomsSessions::OnDestroyCompleted(FName InSessionName, bool bWasSuccessful)
{

}


