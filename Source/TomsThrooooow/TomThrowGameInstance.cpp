// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Sessions/TomsSessions.h"
#include "TomThrowGameInstance.h"




void UTomThrowGameInstance::Init()
{
	Super::Init();
	TomsSession = NewObject<UTomsSessions>();
	TomsSession->InitSessionSystem();
	//InitSessionSystem();
}

