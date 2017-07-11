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
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	StaticMeshComponent->Mobility = EComponentMobility::Movable;
	StaticMeshComponent->bGenerateOverlapEvents = true;

	ProjectileMovementTemplate = NewObject<UProjectileMovementComponent>(this, TEXT("ProjectileMovementTemplate"), 
		RF_ArchetypeObject | RF_Public | RF_Transactional);
	ProjectileMovementTemplate->bAutoRegister = false;
	ProjectileMovementTemplate->bAutoActivate = false;
	ProjectileMovementTemplate->bShouldBounce = true;
	ProjectileMovementTemplate->bInitialVelocityInLocalSpace = false;

	RootComponent = StaticMeshComponent;

	// set replicate
	SetReplicates(true);
	bReplicateMovement = true;
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

void AThrowableActor::OnPick()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Deactivate();
		ProjectileMovement->DestroyComponent();
		ProjectileMovement = NULL;
	}
}

void AThrowableActor::OnThrow(const FVector& ThrowVelocity)
{
	if (!ProjectileMovement)
	{
		ProjectileMovement = Cast<UProjectileMovementComponent>(
			CreateComponentFromTemplate(ProjectileMovementTemplate, FName(TEXT("ProjectileMovement"))));
		ProjectileMovement->OnComponentCreated();
		ProjectileMovement->bAutoActivate = true;
		ProjectileMovement->Velocity = ThrowVelocity;
		ProjectileMovement->RegisterComponent();
	}
}

