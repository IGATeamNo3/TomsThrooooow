// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "TomThrowGameInstance.generated.h"

/**
 * 
 */


UCLASS()
class TOMSTHROOOOOW_API UTomThrowGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
public:
	UFUNCTION(BlueprintPure, Category = Sessions)
		UTomsSessions* GetTomsSession() const { return TomsSession; }
protected:
	UPROPERTY()
	class UTomsSessions* TomsSession;

};

