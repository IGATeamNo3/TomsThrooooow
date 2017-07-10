// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
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
	StaticMeshComponent->bGenerateOverlapEvents = true;

	RootComponent = StaticMeshComponent;

}

// Called when the game starts or when spawned
void AThrowableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowableActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

