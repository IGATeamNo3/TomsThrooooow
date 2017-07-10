// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "GameCharacterBase.generated.h"

UCLASS(abstract, config = Game)
class TOMSTHROOOOOW_API AGameCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AGameCharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/************************************************************************/
	/* Pick                                                                 */
	UPROPERTY(VisibleAnywhere, Category = "Pick")
	UCapsuleComponent* PickCheckCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Pick")
	USceneComponent* PickRoot;

	AActor* PickedActor;
	/************************************************************************/

	/************************************************************************/
	/* Throw                                                                */
	UPROPERTY(EditAnywhere, Category = "Throw")
	float ThrowStrength;
	/************************************************************************/
private:
	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called for look up input */
	void LookUp(float Val);

	/** Called for pick or throw input */
	void PickOrThrow();
};
