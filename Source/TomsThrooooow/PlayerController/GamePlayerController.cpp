// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Camera/DirectorCameraActor.h"
#include "SpeekRecognizerFunctionLibrary.h"
#include "TimelineComment/TimelineCommentManager.h"
#include "GamePlayerController.h"



AGamePlayerController::AGamePlayerController()
{
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// voice text
	InputComponent->BindAction("Speak", IE_Pressed, this, &AGamePlayerController::StartVoiceCapture);
	InputComponent->BindAction("Speak", IE_Released, this, &AGamePlayerController::StopVoiceCapture);
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

void AGamePlayerController::Speak(const FString& Msg)
{
	ServerSpeak(Msg);
}
void AGamePlayerController::ServerSpeak_Implementation(const FString& Msg)
{
	UWorld* CurWorld = GetWorld();
	if (!CurWorld)
	{
		return;
	}

	TSubclassOf<AActor> ActorClass = ATimelineCommentManager::StaticClass();
	if (ActorClass && CurWorld)
	{
		for (TActorIterator<AActor> It(CurWorld, ActorClass); It; ++It)
		{
			ATimelineCommentManager* TimelineCommentManager = Cast<ATimelineCommentManager>(*It);
			if (TimelineCommentManager && !TimelineCommentManager->IsPendingKill())
			{
				TimelineCommentManager->BroadcastVoiceText(Msg);
				return;
			}
		}
	}
}
bool AGamePlayerController::ServerSpeak_Validate(const FString& Msg)
{
	return true;
}

void AGamePlayerController::StartVoiceCapture()
{
	USpeekRecognizerFunctionLibrary::StartLisening();
}

void AGamePlayerController::StopVoiceCapture()
{
	USpeekRecognizerFunctionLibrary::StopLisening();
}
