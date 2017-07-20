// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "TomsThrowLobbyGameMode.h"




ATomsThrowLobbyGameMode::ATomsThrowLobbyGameMode()
{

}

void ATomsThrowLobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTomThrow, Log, TEXT("PreLogin Opitions : %s"), *Options)
	UE_LOG(LogTomThrow, Log, TEXT("PreLogin Address : %s"), *Address)
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
}
