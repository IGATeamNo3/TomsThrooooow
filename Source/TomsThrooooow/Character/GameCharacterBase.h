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
	USceneComponent* PickRoot;	
	
	UPROPERTY(EditAnywhere, Category = "PickAndThrow")
	float ThrowStrength;

	UPROPERTY(EditAnywhere, Category = "PickAndThrow")
	UAnimSequenceBase* PickAnimation;

	UPROPERTY(EditAnywhere, Category = "PickAndThrow")
	UAnimSequenceBase* ThrowAnimation;
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickOrThrow(float RightInput,float UpInput);
	void PickOrThrowWithInput(float RightInput, float UpInput);

	UFUNCTION(Reliable, NetMulticast)
	void BroadcastPickAnimation();
	UFUNCTION(Reliable, NetMulticast)
	void BroadcastThrowAnimation();

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

	UPROPERTY(BlueprintReadOnly, Category = "Stun", ReplicatedUsing = OnRep_SetStunning)
	bool bIsStunning;
private:
	UPROPERTY(VisibleAnywhere, Category = "Stun")
	UParticleSystemComponent* StunEffect;

	UFUNCTION()
	virtual void OnRep_SetStunning();
	/* Timer handle to manage stun time */
	FTimerHandle TimerHandle_Stun;
	/************************************************************************/
};
