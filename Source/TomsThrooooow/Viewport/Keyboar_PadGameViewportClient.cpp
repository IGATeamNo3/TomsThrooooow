// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Keyboar_PadGameViewportClient.h"




bool UKeyboar_PadGameViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed /*= 1.f*/, bool bGamepad /*= false*/)
{
	if (bGamepad)
	{
		// Map the gamepad to the next player index (so 1st controller is player index 1, etc.)
		return Super::InputKey(InViewport, ControllerId + 1, Key, EventType, AmountDepressed, bGamepad);
	}
	else
	{
		return Super::InputKey(InViewport, ControllerId, Key, EventType, AmountDepressed, bGamepad);
	}

}

bool UKeyboar_PadGameViewportClient::InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples /*= 1*/, bool bGamepad /*= false*/)
{
	if (bGamepad)
	{
		// Map the gamepad to the next player index (so 1st controller is player index 1, etc.)
		return Super::InputAxis(InViewport, ControllerId + 1, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
	else
	{
		return Super::InputAxis(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

}
