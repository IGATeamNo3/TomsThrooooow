// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Item/ThrowableInterface.h"
#include "GameCharacterBase.generated.h"

UCLASS(abstract, config = Game)
class TOMSTHROOOOOW_API AGameCharacterBase : public ACharacter , public IThrowableInterface
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AGameCharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickOrThrow(float RightInput,float UpInput);
	void ServerPickOrThrow_Implementation(float RightInput, float UpInput);
	bool ServerPickOrThrow_Validate(float RightInput, float UpInput);
	void PickOrThrowWithInput(float RightInput, float UpInput);

	/** Implement IThrowableInterface start*/
	virtual void OnPick();
	virtual void OnThrow(const FVector& ThrowVelocity);
	/** Implement IThrowableInterface end*/

public:
	// Set the Character state to stun
	void SetStun();
	void SetUnstun();
private:
	UPROPERTY(Transient, Replicated)
	bool bIsStunning;
	/* Timer handle to manage stun time */
	FTimerHandle TimerHandle_Stun;
};
