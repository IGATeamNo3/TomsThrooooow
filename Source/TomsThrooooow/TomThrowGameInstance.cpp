// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "TomThrowGameInstance.h"


const FName SessionName(TEXT("Game"));

void UTomThrowGameInstance::Init()
{
	Super::Init();
}

void UTomThrowGameInstance::CreateSessionWithSetting(APlayerController* PC,  bool bUseLAN)
{
	IOnlineSubsystem* OSSPtr = IOnlineSubsystem::Get();
	
	FOnlineSessionSettings Settings;
	
	Settings.bIsLANMatch = bUseLAN;
	
	TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);
	
	OSSPtr->GetSessionInterface()->CreateSession(*UserID, SessionName, Settings);
	
}

void UTomThrowGameInstance::FindSessionsWithSetting(APlayerController* PC, int32 MaxResults,  bool bUseLAN)
{
	IOnlineSubsystem* OSSPtr = IOnlineSubsystem::Get();
	TSharedPtr<const FUniqueNetId> UserID = GetPlayerUniqueID(PC);

	TSharedPtr<FOnlineSessionSearch> SearchObject = MakeShareable(new FOnlineSessionSearch);
	SearchObject->MaxSearchResults = MaxResults;
	SearchObject->bIsLanQuery = bUseLAN;
	SearchObject->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	OSSPtr->GetSessionInterface()->FindSessions(*UserID, SearchObject.ToSharedRef());
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
