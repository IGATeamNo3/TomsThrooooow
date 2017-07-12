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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Implement IThrowableInterface start*/
	virtual void OnPick();
	virtual void OnThrow(const FVector& ThrowVelocity);
	/** Implement IThrowableInterface end*/

	UFUNCTION()
	virtual void OnCollisionHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(Category = ThrowableActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(Category = ThrowableActor, VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovementTemplate;

	class UProjectileMovementComponent* ProjectileMovement;
};
