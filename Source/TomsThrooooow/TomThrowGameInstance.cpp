// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "TomThrowGameInstance.h"


const FName SessionName(TEXT("Game"));

void UTomThrowGameInstance::Init()
{
	Super::Init();
	InitSessionSystem();
}

void UTomThrowGameInstance::CreateSessionWithSetting(APlayerController* PC,  bool bUseLAN)
{
	IOnlineSubsystem* OSSPtr = IOnlineSubsystem::Get();
	
	if (OSSPtr)
	{
		FOnlineSessionSettings Settings;

		Settings.bIsLANMatch = bUseLAN;

		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);

		CreateCompleteDelegateHandle = OSSPtr->GetSessionInterface()->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);
		OSSPtr->GetSessionInterface()->CreateSession(*UserID, SessionName, Settings);
	}	
}

void UTomThrowGameInstance::FindSessionsWithSetting(APlayerController* PC, int32 MaxResults,  bool bUseLAN)
{
	IOnlineSubsystem* OSSPtr = IOnlineSubsystem::Get();
	if (OSSPtr)
	{
		TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);

		TSharedPtr<FOnlineSessionSearch> SearchObject = MakeShareable(new FOnlineSessionSearch);
		SearchObject->MaxSearchResults = MaxResults;
		SearchObject->bIsLanQuery = bUseLAN;
		SearchObject->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		OSSPtr->GetSessionInterface()->FindSessions(*UserID, SearchObject.ToSharedRef());
	}
	
}

TSharedPtr<const FUniqueNetId> UTomThrowGameInstance::GetPlayerUniqueID(APlayerController* PlayerController)
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

void UTomThrowGameInstance::HandleCreateSessionComplete(FName InSessionName, bool bSuccess)
{

}

void UTomThrowGameInstance::HandleFindSessionsComplete(bool bSuccess)
{

}

void UTomThrowGameInstance::HandleJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type ResultType)
{

}

void UTomThrowGameInstance::InitSessionSystem()
{
	CreateCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted);
	StartCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted);
}

void UTomThrowGameInstance::OnCreateCompleted(FName InSessionName, bool bSuccess)
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

void UTomThrowGameInstance::OnStartCompleted(FName InSessionName, bool bSuccess)
{
	IOnlineSubsystem* Sessions = IOnlineSubsystem::Get();

	if (Sessions)
	{
		Sessions->GetSessionInterface()->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
		if (bSuccess)
		{
			OnCreateComplete.Broadcast(ECompeleteResult::EC_Success);
		}
	}
	
	if (!bSuccess)
	{
		OnCreateComplete.Broadcast(ECompeleteResult::EC_Failure);
	}

}
