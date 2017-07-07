// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"
#include "AutoAdjustmentCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class TOMSTHROOOOOW_API AAutoAdjustmentCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	AAutoAdjustmentCameraActor();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void UpdateCameraTransform(float DeltaSeconds);
	UFUNCTION(BlueprintPure, Category = "Game | GameCamera")
	int32 GetLocalGamePlayerNumber();
	const TArray<ULocalPlayer*> GetLocalGamePlayers() const;
	UFUNCTION(BlueprintCallable, Category = "Game | GameCamera")
	void SetViewTargetToThisCamera();
	void SetViewTargetToThisCamera(APlayerController* PlayerController);

private:
	FVector GetMaxXPoint();
	FVector GetMinXPoint();
	FVector GetMaxYPoint();
	FVector GetMinYPoint();
	FVector GetMaxZPoint();
	FVector GetMinZPoint();
	void UpdatePlayersLocation();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Camera")
	float InterSpeed;
private:
	TArray<FVector> PlayersLocation;
	
	
};
