// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/GameCharacterBase.h"
#include "DirectorCameraActor.h"



ADirectorCameraActor::ADirectorCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MoveType = EDirectorCameraMoveType::Fix;
	InterSpeed = 10.f;
	ClosestDistance = 600.f;
}

void ADirectorCameraActor::Tick(float DeltaSeconds)
{
	if (MoveType == EDirectorCameraMoveType::Fix)
	{
		return;
	}

	UWorld* CurWorld = GetWorld();
	if (CurWorld)
	{
		TArray<AGameCharacterBase*> AllGameCharacters;
		TArray<FVector> AllGameCharactersPosition;
		for (TActorIterator<AGameCharacterBase> It(CurWorld); It; ++It)
		{
			AGameCharacterBase* GameCharacterBase = *It;
			if (!GameCharacterBase->IsPendingKill())
			{
				AllGameCharacters.AddUnique(GameCharacterBase);
				AllGameCharactersPosition.Add(GameCharacterBase->GetActorLocation());
			}
		}

		// get all pawns center position first
		FVector DestinationLocation = UKismetMathLibrary::GetVectorArrayAverage(AllGameCharactersPosition);

		if (AllGameCharacters.Num() == 0)
		{
			return;
		}
		else if (AllGameCharacters.Num() == 1)
		{
			AGameCharacterBase* GameCharacterBase = AllGameCharacters[0];
			DestinationLocation.X = GameCharacterBase->GetActorLocation().X + ClosestDistance;
		}
		else
		{
			float MaxWidth = 0;
			float MaxHeight = 0;
			for (int i = 0; i < AllGameCharacters.Num(); ++i)
			{
				// TODO:1.2 is magic number
				float CurrentWidth = UKismetMathLibrary::Abs(AllGameCharacters[i]->GetActorLocation().Y - DestinationLocation.Y) * 1.2;
				float CurrentHeight = UKismetMathLibrary::Abs(AllGameCharacters[i]->GetActorLocation().Z - DestinationLocation.Z) * 1.2;
				MaxWidth = UKismetMathLibrary::FMax(MaxWidth, CurrentWidth);
				MaxHeight = UKismetMathLibrary::FMax(MaxHeight, CurrentHeight);
			}

			MaxWidth = UKismetMathLibrary::FMax(MaxWidth, MaxHeight * GetCameraComponent()->AspectRatio);
			float Distance = MaxWidth / UKismetMathLibrary::DegTan(GetCameraComponent()->FieldOfView / 2);
			Distance = UKismetMathLibrary::FMax(Distance, ClosestDistance);

			DestinationLocation.X = DestinationLocation.X + Distance;

			//UE_LOG(LogTomThrow, Verbose, TEXT("MaxWidth:(%f) Distance:(%f)"), MaxWidth, Distance);
		}

		SetActorLocation(FMath::VInterpTo(GetActorLocation(), DestinationLocation, DeltaSeconds, InterSpeed));
	}
}

