// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item/ThrowableActor.h"
#include "GameCharacterBase.h"


// Sets default values
AGameCharacterBase::AGameCharacterBase(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 7200.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Configure PickCheckCapsule
	PickCheckCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PickCheckCapsule"));
	PickCheckCapsule->SetRelativeLocation(FVector(90, 0, 0));
	PickCheckCapsule->SetCapsuleHalfHeight(60);
	PickCheckCapsule->SetCapsuleRadius(35, false);
	PickCheckCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickCheckCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickCheckCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	PickCheckCapsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	PickCheckCapsule->SetupAttachment(GetCapsuleComponent());

	// Configure PickRoot
	PickRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickRoot"));
	PickRoot->SetRelativeLocation(FVector(0, 0, 180));
	PickRoot->SetupAttachment(GetCapsuleComponent());

	// Property Initialize
	ThrowStrength = 1000.f;
}

// Called to bind functionality to input
void AGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("PickOrThrow", IE_Pressed, this, &AGameCharacterBase::PickOrThrow);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AGameCharacterBase::LookUp);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGameCharacterBase::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AGameCharacterBase::LookUp(float Value)
{
	//do nothing now
}

void AGameCharacterBase::PickOrThrow()
{
	if (Role < ROLE_Authority)
	{
		ServerPickOrThrow();
		return;
	}

	// if PickedActor exists, then throw it
	if (PickedActor)
	{
		UPrimitiveComponent* PickedActorPrimitiveComponent = Cast<UPrimitiveComponent>(PickedActor->GetRootComponent());
		if (PickedActorPrimitiveComponent)
		{
			// Detach to PickRoot
			PickedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PickedActorPrimitiveComponent->SetSimulatePhysics(true);

			float MoveRightInputAxisValue = GetInputAxisValue("MoveRight");
			float LookUpInputAxisValue = GetInputAxisValue("LookUp");

			// without input throw
			FVector ThrowVector;
			if (MoveRightInputAxisValue == .0f && LookUpInputAxisValue == .0f)
			{
				ThrowVector = GetArrowComponent()->GetForwardVector() * ThrowStrength;
			}
			// with input throw
			else
			{
				float InputDegree = UKismetMathLibrary::DegAtan2(LookUpInputAxisValue, MoveRightInputAxisValue);
				FRotator InputRotator = UKismetMathLibrary::MakeRotator(InputDegree, 0.f, 0.f);
				ThrowVector = UKismetMathLibrary::GreaterGreater_VectorRotator(FVector(0.f, -1.f, 0.f), InputRotator) * ThrowStrength;
			}

			PickedActorPrimitiveComponent->SetPhysicsLinearVelocity(ThrowVector, true);
			
		}

		PickedActor = NULL;
	}
	// if PickedActor not exists, then try pick up something
	else
	{
		TArray<AActor*> Overlaps;
		PickCheckCapsule->GetOverlappingActors(Overlaps, AThrowableActor::StaticClass());
		for (int32 i = 0; i < Overlaps.Num(); i++)
		{
			if (Overlaps[i] && !Overlaps[i]->IsPendingKill())
			{
				PickedActor = Overlaps[i];

				UPrimitiveComponent* PickedActorPrimitiveComponent = Cast<UPrimitiveComponent>(PickedActor->GetRootComponent());
				if (PickedActorPrimitiveComponent)
				{
					// Attach to PickRoot
					PickedActorPrimitiveComponent->SetSimulatePhysics(false);
					PickedActor->AttachToComponent(PickRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					break;
				}
			}
		}
	}
}

void AGameCharacterBase::ServerPickOrThrow_Implementation()
{
	PickOrThrow();
}

bool AGameCharacterBase::ServerPickOrThrow_Validate()
{
	return true;
}

