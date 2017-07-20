// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThrowableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UThrowableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class TOMSTHROOOOOW_API IThrowableInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnPick() = 0;

	virtual void OnThrow(const FVector& ThrowVelocity) = 0;
	
};
