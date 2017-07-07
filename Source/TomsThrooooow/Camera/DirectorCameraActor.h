// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"
#include "DirectorCameraActor.generated.h"

/**
*
*/
UENUM()
enum class EDirectorCameraMoveType : uint8
{
	INVALID,
	/** The Camera don't move ever */
	Fix,
	/** The Camera will adjust itself to cover all pawns */
	AlwaysCoverAllPawns /** TODO */
};


UCLASS()
class TOMSTHROOOOOW_API ADirectorCameraActor : public ACameraActor
{
	GENERATED_BODY()
public:
	ADirectorCameraActor();
private:
	UPROPERTY(EditAnywhere, Category = Director)
	EDirectorCameraMoveType MoveType;
};

