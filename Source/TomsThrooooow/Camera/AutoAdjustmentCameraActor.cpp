// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "AutoAdjustmentCameraActor.h"




AAutoAdjustmentCameraActor::AAutoAdjustmentCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
	InterSpeed = 1.0f;
}

void AAutoAdjustmentCameraActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdatePlayersLocation();
	UpdateCameraTransform( DeltaSeconds);
}

void AAutoAdjustmentCameraActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAutoAdjustmentCameraActor::UpdateCameraTransform(float DeltaSeconds)
{
	FVector  Center= FVector((GetMaxXPoint()+GetMinXPoint()).X*0.5f, 
		(GetMaxYPoint() + GetMinYPoint()).Y*0.5f,
		(GetMaxZPoint() + GetMinZPoint()).Z*0.5f);

	FMath::VInterpTo(GetActorLocation(), Center, DeltaSeconds, InterSpeed);
	SetActorLocation(Center);
}

int32 AAutoAdjustmentCameraActor::GetLocalGamePlayerNumber()
{
	TArray<ULocalPlayer*> LocalPlayers = GetLocalGamePlayers();
	return LocalPlayers.Num();
}

void AAutoAdjustmentCameraActor::SetViewTargetToThisCamera()
{
	TArray<ULocalPlayer*> LocalPlayers = GetLocalGamePlayers();
	for (int8 i = 0; i < LocalPlayers.Num(); i++)
	{
		LocalPlayers[i]->PlayerController->SetViewTargetWithBlend(this);
	}

}

void AAutoAdjustmentCameraActor::SetViewTargetToThisCamera(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(this);
	}
}

FVector AAutoAdjustmentCameraActor::GetMaxXPoint()
{
	FVector ret;
	if (PlayersLocation.Num() > 0)
	{
		ret = PlayersLocation[0];
		for (int8 i = 1; i < PlayersLocation.Num(); i++)
		{
			if (PlayersLocation[i].X > ret.X)
			{
				ret = PlayersLocation[i];
			}
		}
	}
	return ret;
}

FVector AAutoAdjustmentCameraActor::GetMinXPoint()
{
	FVector ret;
	if (PlayersLocation.Num() > 0)
	{
		ret = PlayersLocation[0];
		for (int8 i = 1; i < PlayersLocation.Num(); i++)
		{
			if (PlayersLocation[i].X < ret.X)
			{
				ret = PlayersLocation[i];
			}
		}
	}
	return ret;
}

FVector AAutoAdjustmentCameraActor::GetMaxYPoint()
{
	FVector ret;
	if (PlayersLocation.Num() > 0)
	{
		ret = PlayersLocation[0];
		for (int8 i = 1; i < PlayersLocation.Num(); i++)
		{
			if (PlayersLocation[i].Y > ret.Y)
			{
				ret = PlayersLocation[i];
			}
		}
	}
	return ret;
}

FVector AAutoAdjustmentCameraActor::GetMinYPoint()
{
	FVector ret;
	if (PlayersLocation.Num() > 0)
	{
		ret = PlayersLocation[0];
		for (int8 i = 1; i < PlayersLocation.Num(); i++)
		{
			if (PlayersLocation[i].Y < ret.Y)
			{
				ret = PlayersLocation[i];
			}
		}
	}
	return ret;
}

FVector AAutoAdjustmentCameraActor::GetMaxZPoint()
{
	FVector ret;
	if (PlayersLocation.Num() > 0)
	{
		ret = PlayersLocation[0];
		for (int8 i = 1; i < PlayersLocation.Num(); i++)
		{
			if (PlayersLocation[i].Z > ret.Z)
			{
				ret = PlayersLocation[i];
			}
		}
	}
	return ret;
}

FVector AAutoAdjustmentCameraActor::GetMinZPoint()
{
	FVector ret;
	if (PlayersLocation.Num() > 0)
	{
		ret = PlayersLocation[0];
		for (int8 i = 1; i < PlayersLocation.Num();i++)
		{
			if (PlayersLocation[i].Z < ret.Z)
			{
				ret = PlayersLocation[i];
			}
		}
	}
	return ret;
}

void AAutoAdjustmentCameraActor::UpdatePlayersLocation()
{
	PlayersLocation.Reset();
	TArray<ULocalPlayer*> LocalPlayers = GetLocalGamePlayers();
	for (int8 i = 0; i < LocalPlayers.Num(); i++)
	{	
		if (LocalPlayers[i] && LocalPlayers[i]->PlayerController)
		{
			APawn* pPawn = LocalPlayers[i]->PlayerController->GetPawn();
			if (pPawn)
			{
				PlayersLocation.Add(pPawn->GetActorLocation());
			}
		}
		
	}
}

const TArray<ULocalPlayer*> AAutoAdjustmentCameraActor::AAutoAdjustmentCameraActor::GetLocalGamePlayers() const
{
	return GEngine->GetGamePlayers(GetWorld());
}
