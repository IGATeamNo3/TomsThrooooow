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
		FVector SelfVelocity = GetVelocity();
		//UE_LOG(LogTomThrow, Verbose, TEXT("SelfVelocity:(%f,%f,%f)"), SelfVelocity.X, SelfVelocity.Y, SelfVelocity.Z);

		if (SelfVelocity.Size() > 200 && NormalImpulse.Size() > 100000)
		{
			OtherCharacter->SetStun();
		}

		//FVector SelfVelocity = GetVelocity();
		//OtherCharacter->GetCharacterMovement()->AddImpulse(SelfVelocity * -1,true);
		
		//OtherCharacter->GetCharacterMovement()->AddImpulse(NormalImpulse, false);
		//UE_LOG(LogTomThrow, Verbose, TEXT("NormalImpulse:(%f,%f,%f)"), NormalImpulse.X, NormalImpulse.Y, NormalImpulse.Z);
	}

	if (bCrushableOnHit && CrushEffect && NormalImpulse.Size() > CrushThreshold)
	{
		FTransform SpawnTransform = GetActorTransform();
		FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);
		SpawnTransform.SetScale3D(SpawnScale);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CrushEffect, SpawnTransform, true);

		Destroy();
	}
}

