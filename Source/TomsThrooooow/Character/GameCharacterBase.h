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
	AGameCharacterBase(const class FObjectInitializer& ObjectInitializer);

	/************************************************************************/
	/* Input Bind                                                           */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	void MoveRight(float Val);
	void LookUp(float Val);
	void PickOrThrow();
	/************************************************************************/

	/************************************************************************/
	/* Pick And Throw                                                       */
	AActor* PickedActor;

	UPROPERTY(Transient, Replicated)
	bool bIsPickedByOthers;

	UPROPERTY(VisibleAnywhere, Category = "PickAndThrow")
	UCapsuleComponent* PickCheckCapsule;

	UPROPERTY(VisibleAnywhere, Category = "PickAndThrow")
	UCapsuleComponent* HitCheckCapsule;

	UPROPERTY(VisibleAnywhere, Category = "PickAndThrow")
	USceneComponent* PickRoot;	
	
	UPROPERTY(EditAnywhere, Category = "PickAndThrow")
	float ThrowStrength;
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickOrThrow(float RightInput,float UpInput);
	void PickOrThrowWithInput(float RightInput, float UpInput);

	/** Implement IThrowableInterface start*/
	virtual void OnPick();
	virtual void OnThrow(const FVector& ThrowVelocity);
	/** Implement IThrowableInterface end*/
	/************************************************************************/

	/************************************************************************/
	/* Stun                                                                 */
public:
	// Set the Character state to stun
	void SetStun();
	void SetUnstun();
private:
	UPROPERTY(VisibleAnywhere, Category = "PickAndThrow")
	UParticleSystemComponent* StunEffect;

	UFUNCTION()
	virtual void OnRep_SetStunning();

	UPROPERTY(Transient, ReplicatedUsing=OnRep_SetStunning)
	bool bIsStunning;
	/* Timer handle to manage stun time */
	FTimerHandle TimerHandle_Stun;
	/************************************************************************/
};
