// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Camera/DirectorCameraActor.h"
#include "GamePlayerController.h"



AGamePlayerController::AGamePlayerController()
{
}

void AGamePlayerController::AutoManageActiveCameraTarget(AActor* SuggestedTarget)
{
	if (bAutoManageActiveCameraTarget)
	{
		UWorld* CurWorld = GetWorld();
		if (!CurWorld)
		{
			return;
		}

		TSubclassOf<AActor> ActorClass = ADirectorCameraActor::StaticClass();
		if (ActorClass && CurWorld)
		{
			for (TActorIterator<AActor> It(CurWorld, ActorClass); It; ++It)
			{
				AActor* Actor = *It;
				if (!Actor->IsPendingKill())
				{
					// if find ADirectorCameraActor, use it
					SetViewTarget(Actor);
					return;
				}
			}
		}

		// No auto-activate CameraActor, so use the suggested target.
		SetViewTarget(SuggestedTarget);
	}
}
