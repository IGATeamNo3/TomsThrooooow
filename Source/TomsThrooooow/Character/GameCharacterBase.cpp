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
	GetCharacterMovement()->bEnablePhysicsInteraction = false;

	// Configure PickCheckCapsule
	PickCheckCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PickCheckCapsule"));
	PickCheckCapsule->SetRelativeLocation(FVector(90, 0, 0));
	PickCheckCapsule->SetCapsuleHalfHeight(60);
	PickCheckCapsule->SetCapsuleRadius(35, false);
	PickCheckCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickCheckCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickCheckCapsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	PickCheckCapsule->SetupAttachment(GetCapsuleComponent());

	// Configure PickRoot
	PickRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickRoot"));
	PickRoot->SetRelativeLocation(FVector(0, 0, 180));
	PickRoot->SetupAttachment(GetCapsuleComponent());

	// Property Initialize
	ThrowStrength = 1000.f;
}

void AGameCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameCharacterBase, bIsStunning);
}

// Called to bind functionality to input
void AGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGameCharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("PickOrThrow", IE_Pressed, this, &AGameCharacterBase::PickOrThrow);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AGameCharacterBase::LookUp);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGameCharacterBase::Jump()
{
	if (!bIsStunning)
	{
		Super::Jump();
	}
}

void AGameCharacterBase::MoveRight(float Value)
{
	if (!bIsStunning)
	{
		// add movement in that direction
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
	}
}

void AGameCharacterBase::LookUp(float Value)
{
	//do nothing now
}

void AGameCharacterBase::PickOrThrow()
{
	if (!bIsStunning)
	{
		float MoveRightInputAxisValue = GetInputAxisValue("MoveRight");
		float LookUpInputAxisValue = GetInputAxisValue("LookUp");

		if (Role < ROLE_Authority)
		{
			ServerPickOrThrow(MoveRightInputAxisValue, LookUpInputAxisValue);
			return;
		}

		PickOrThrowWithInput(MoveRightInputAxisValue, LookUpInputAxisValue);
	}
}

void AGameCharacterBase::ServerPickOrThrow_Implementation(float RightInput, float UpInput)
{

	PickOrThrowWithInput(RightInput, UpInput);
}

bool AGameCharacterBase::ServerPickOrThrow_Validate(float RightInput, float UpInput)
{
	return true;
}

void AGameCharacterBase::PickOrThrowWithInput(float RightInput, float UpInput)
{
	// if PickedActor exists, then throw it
	if (PickedActor)
	{
		IThrowableInterface* ThrowableActor = Cast<IThrowableInterface>(PickedActor);
		if (ThrowableActor)
		{
			// Detach to PickRoot
			PickedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			// without input throw
			FVector ThrowVector;
			if (RightInput == .0f && UpInput == .0f)
			{
				ThrowVector = GetArrowComponent()->GetForwardVector() * ThrowStrength;
			}
			// with input throw
			else
			{
				float InputDegree = UKismetMathLibrary::DegAtan2(UpInput, RightInput);
				FRotator InputRotator = UKismetMathLibrary::MakeRotator(InputDegree, 0.f, 0.f);
				ThrowVector = UKismetMathLibrary::GreaterGreater_VectorRotator(FVector(0.f, -1.f, 0.f), InputRotator) * ThrowStrength;
			}

			ThrowableActor->OnThrow(ThrowVector);
		}

		PickedActor = NULL;
	}
	// if PickedActor not exists, then try pick up something
	else
	{
		// check for character pick
		{
			TArray<AActor*> Overlaps;
			PickCheckCapsule->GetOverlappingActors(Overlaps, AGameCharacterBase::StaticClass());
			for (int32 i = 0; i < Overlaps.Num(); i++)
			{
				if (Overlaps[i] && !Overlaps[i]->IsPendingKill())
				{
					PickedActor = Overlaps[i];

					AGameCharacterBase* ThrowableActor = Cast<AGameCharacterBase>(PickedActor);
					if (ThrowableActor)
					{
						// Attach to PickRoot
						ThrowableActor->OnPick();
						ThrowableActor->AttachToComponent(PickRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						return;
					}
				}
			}
		}

		// check for throwable actor pick
		{
			TArray<AActor*> Overlaps;
			PickCheckCapsule->GetOverlappingActors(Overlaps, AThrowableActor::StaticClass());
			for (int32 i = 0; i < Overlaps.Num(); i++)
			{
				if (Overlaps[i] && !Overlaps[i]->IsPendingKill())
				{
					PickedActor = Overlaps[i];

					AThrowableActor* ThrowableActor = Cast<AThrowableActor>(PickedActor);
					if (ThrowableActor)
					{
						// Attach to PickRoot
						ThrowableActor->OnPick();
						ThrowableActor->AttachToComponent(PickRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						return;
					}
				}
			}
		}
	}
}

void AGameCharacterBase::OnPick()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCapsuleComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}
void AGameCharacterBase::OnThrow(const FVector& ThrowVelocity)
{
	LaunchCharacter(ThrowVelocity, false, false);
	GetCapsuleComponent()->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
}

void AGameCharacterBase::SetStun()
{
	bIsStunning = true;

	GetWorldTimerManager().ClearTimer(TimerHandle_Stun);
	GetWorldTimerManager().SetTimer(TimerHandle_Stun, this, &AGameCharacterBase::SetUnstun, 5.f);
}
void AGameCharacterBase::SetUnstun()
{
	bIsStunning = false;
}

