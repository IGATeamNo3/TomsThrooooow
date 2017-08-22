// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "DirectorCameraActor.h"



ADirectorCameraActor::ADirectorCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MoveType = EDirectorCameraMoveType::Fix;
	InterSpeed = 10.f;
}

void ADirectorCameraActor::Tick(float DeltaSeconds)
{
	if (MoveType == EDirectorCameraMoveType::Fix)
	{
		return;
	}

	TArray<ULocalPlayer*> LocalPlayers = GEngine->GetGamePlayers(GetWorld());

	FVector DestinationLocation = GetActorLocation();
	if (LocalPlayers.Num() == 1)
	{
		if (LocalPlayers[0] && LocalPlayers[0]->PlayerController)
		{
			APawn* pPawn = LocalPlayers[0]->PlayerController->GetPawn();
			if (pPawn)
			{
				DestinationLocation = FVector(GetActorLocation().X, pPawn->GetActorLocation().Y, pPawn->GetActorLocation().Z);
			}
		}
	}

	SetActorLocation(FMath::VInterpTo(GetActorLocation(), DestinationLocation, DeltaSeconds, InterSpeed));
}

