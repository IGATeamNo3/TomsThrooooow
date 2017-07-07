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
	/** always set player camera to director camera if exists */
	virtual void AutoManageActiveCameraTarget(AActor* SuggestedTarget) override;
};
