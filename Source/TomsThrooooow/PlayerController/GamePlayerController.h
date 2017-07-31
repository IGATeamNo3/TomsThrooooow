// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
*
*/
UCLASS()
class TOMSTHROOOOOW_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	AGamePlayerController();
public:
	/** sets up input */
	virtual void SetupInputComponent() override;
	/** always set player camera to director camera if exists */
	virtual void AutoManageActiveCameraTarget(AActor* SuggestedTarget) override;

	/** the invoke of Speak is in the level blueprint, cause one compute have only one microphone */
	UFUNCTION(BlueprintCallable, Category = VoiceText)
	void Speak(const FString& Msg);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpeak(const FString& Msg);
private:
	void StartVoiceCapture();
	void StopVoiceCapture();
};
