// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "AutoCameraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOMSTHROOOOOW_API AAutoCameraPlayerController : public APlayerController
{
	GENERATED_BODY()
		AAutoCameraPlayerController();
public:
	virtual void Possess(APawn* aPawn) override;
	virtual void ClientRestart_Implementation(APawn* NewPawn) override;
protected:
	void SetViewTargetToAutoCamera();
};
