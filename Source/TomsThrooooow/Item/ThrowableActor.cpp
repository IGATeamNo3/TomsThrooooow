// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Character/GameCharacterBase.h"
#include "ThrowableActor.h"


// Sets default values
AThrowableActor::AThrowableActor(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	StaticMeshComponent->Mobility = EComponentMobility::Movable;
	StaticMeshComponent->BodyInstance.bSimulatePhysics = true;
	StaticMeshComponent->BodyInstance.bNotifyRigidBodyCollision = true;
	StaticMeshComponent->BodyInstance.bLockXTranslation = true;
	StaticMeshComponent->bGenerateOverlapEvents = true;

	RootComponent = StaticMeshComponent;

	// set event callback
	OnActorHit.AddDynamic(this, &AThrowableActor::OnCollisionHit);

	// set replicate
	SetReplicates(true);
	bReplicateMovement = true;
}

void AThrowableActor::OnPick()
{
	StaticMeshComponent->SetSimulatePhysics(false);
}

void AThrowableActor::OnThrow(const FVector& ThrowVelocity)
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetPhysicsLinearVelocity(ThrowVelocity, true);
}

void AThrowableActor::OnCollisionHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	AGameCharacterBase* OtherCharacter = Cast<AGameCharacterBase>(OtherActor);
	// if hit is a character
	if (OtherCharacter)
	{
		//OtherCharacter->SetStun();

		FVector SelfVelocity = GetVelocity();
		OtherCharacter->LaunchCharacter(SelfVelocity, false, false);
	}
}

