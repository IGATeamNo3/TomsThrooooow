// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Camera/AutoAdjustmentCameraActor.h"
#include "AutoCameraPlayerController.h"



AAutoCameraPlayerController::AAutoCameraPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void AAutoCameraPlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);
	SetViewTargetToAutoCamera();
}

void AAutoCameraPlayerController::ClientRestart_Implementation(APawn* NewPawn)
{
	Super::ClientRestart_Implementation(NewPawn);
	SetViewTargetToAutoCamera();
}

void AAutoCameraPlayerController::SetViewTargetToAutoCamera()
{
	UWorld* World = GetWorld();
	TSubclassOf<AActor> AutoCameraClass = AAutoAdjustmentCameraActor::StaticClass();
	// We do nothing if not class provided, rather than giving ALL actors!
	if (AutoCameraClass && World)
	{
		for (TActorIterator<AActor> It(World, AutoCameraClass); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->IsPendingKill())
			{
				AAutoAdjustmentCameraActor* Camera = Cast<AAutoAdjustmentCameraActor>(Actor);
				if (Camera)
				{
					Camera->SetViewTargetToThisCamera(this);
				}
				break;
			}
		}
	}
}

