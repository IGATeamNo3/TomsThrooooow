// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "TomsSessions.h"


//const FName TomsSessionName(TEXT("TomsThrowSession"));
//const FName TomsSessionName(TEXT("Game"));
//Mark GetNamedSession
UTomsSessions::UTomsSessions()
{
	UE_LOG(LogTomThrow, Warning, TEXT("New TomsSession "))
}

UTomsSessions::~UTomsSessions()
{
	UE_LOG(LogTomThrow, Warning, TEXT("Desroy TomsSession "))
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();
	if (Sessions)
	{
		DestroyCompleteDelegateHandle = Sessions->GetSessionInterface()->AddOnDestroySessionCompleteDelegate_Handle(DestroyCompleteDelegate);
		Sessions->GetSessionInterface()->DestroySession(GameSessionName);
	}
}

void UTomsSessions::CreateSessionWithSetting(UObject* WorldContextObject, APlayerController* PC, bool bUseLAN)
{
	
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(WorldContextObject->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		World = WorldContextObject;
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);
		if (Sessions.IsValid() && UserID.IsValid())
		{
			FOnlineSessionSettings Settings;
			Settings.NumPublicConnections = 10;
			Settings.bShouldAdvertise = true;
			Settings.bAllowJoinInProgress = true;
			Settings.bUsesPresence = true;
			Settings.bAllowJoinViaPresence = true;
			Settings.bIsLANMatch = bUseLAN;

			CreateCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);
			Sessions->CreateSession(*UserID, GameSessionName, Settings);
		}
	
	}
}

void UTomsSessions::FindSessionsWithSetting(UObject* WorldContextObject, APlayerController* PC, int32 MaxResults, bool bUseLAN)
{
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(WorldContextObject->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		World = WorldContextObject;
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);
		if (Sessions.IsValid() && UserID.IsValid())
		{
			SearchObject = MakeShareable(new FOnlineSessionSearch);
			if (SearchObject.IsValid())
			{
				SearchObject->MaxSearchResults = MaxResults;
				SearchObject->bIsLanQuery = bUseLAN;
				SearchObject->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
				FindCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(FindCompleteDelegate);
				Sessions->FindSessions(*UserID, SearchObject.ToSharedRef());
			}
		}
	}
}

void UTomsSessions::JoinSessionWithSetting(UObject* WorldContextObject, APlayerController* PC, const FTomsBlueprintSessionResult& Session)
{
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(WorldContextObject->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		World = WorldContextObject;
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);
		if (Sessions.IsValid() && UserID.IsValid())
		{
			JoinCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(JoinCompleteDelegate);
			Sessions->JoinSession(*UserID, GameSessionName, Session.OnlineResult);
		}
	}
}

void UTomsSessions::DestroyTomsSession(UObject* WorldContextObject, APlayerController* PC)
{
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(WorldContextObject->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		World = WorldContextObject;
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);
		if (Sessions.IsValid() && UserID.IsValid())
		{
			DestroyCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(DestroyCompleteDelegate);
			Sessions->DestroySession(GameSessionName);
		}
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
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(World->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);
			if (bSuccess)
			{
				StartCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
				Sessions->StartSession(InSessionName);
				return;
			}
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
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(World->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
			if (bSuccess)
			{
				OnCreateComplete.Broadcast(ECompeleteResult::EC_Success);
				return;
			}
		}
	}

	if (!bSuccess)
	{
		OnCreateComplete.Broadcast(ECompeleteResult::EC_Failure);
	}

}

void UTomsSessions::OnFindCompleted(bool bSuccess)
{
	TArray<FTomsBlueprintSessionResult> Results;
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(World->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(FindCompleteDelegateHandle);

			if (bSuccess && SearchObject.IsValid())
			{
				for (auto& Result : SearchObject->SearchResults)
				{
					FTomsBlueprintSessionResult BPResult;
					BPResult.OnlineResult = Result;
					Results.Add(BPResult);
				}

				OnFindComplete.Broadcast(ECompeleteResult::EC_Success, Results);
				return;
			}
		}
	}
	OnFindComplete.Broadcast(ECompeleteResult::EC_Failure, Results);
}
void UTomsSessions::OnJoinCompleted(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(World->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(JoinCompleteDelegateHandle);
			FString ConnectString;
			if (Sessions->GetResolvedConnectString(GameSessionName, ConnectString))
			{
				UE_LOG(LogTomThrow, Log, TEXT("Join Session: Traveling to %s"), *ConnectString)
					OnJoinComplete.Broadcast(ECompeleteResult::EC_Success);
				return;
			}
		}
	}
	OnJoinComplete.Broadcast(ECompeleteResult::EC_Failure);

}

void UTomsSessions::OnDestroyCompleted(FName InSessionName, bool bSuccess)
{
	IOnlineSubsystem* const OSSPtr = Online::GetSubsystem(World->GetWorld());
	if (OSSPtr)
	{
		IOnlineSessionPtr Sessions = OSSPtr->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroyCompleteDelegateHandle);
			if (bSuccess)
			{
				OnDestroyComplete.Broadcast(ECompeleteResult::EC_Success);
				return;
			}
		}
	}

	OnDestroyComplete.Broadcast(ECompeleteResult::EC_Failure);

}


