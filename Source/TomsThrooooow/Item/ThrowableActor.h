// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Item/ThrowableInterface.h"
#include "ThrowableActor.generated.h"

UCLASS(abstract)
class TOMSTHROOOOOW_API AThrowableActor : public AActor , public IThrowableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableActor(const class FObjectInitializer& ObjectInitializer);

	/** Implement IThrowableInterface start*/
	virtual void OnPick();
	virtual void OnThrow(const FVector& ThrowVelocity);
	/** Implement IThrowableInterface end*/

	UFUNCTION()
	virtual void OnCollisionHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(Category = ThrowableActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(Category = "ThrowableActor | Crush", EditAnywhere)
	bool bCrushableOnHit;

	UPROPERTY(Category = "ThrowableActor | Crush", EditAnywhere, meta = (EditCondition = "bCrushableOnHit"))
	float CrushThreshold;

	UPROPERTY(Category = "ThrowableActor | Crush", EditAnywhere , meta = (EditCondition = "bCrushableOnHit"))
	class UParticleSystem* CrushEffect;


};
