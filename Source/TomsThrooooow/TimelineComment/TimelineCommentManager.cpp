// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Camera/DirectorCameraActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimelineCommentEntry.h"
#include "TimelineCommentManager.h"


// Sets default values
ATimelineCommentManager::ATimelineCommentManager(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->Mobility = EComponentMobility::Movable;
#if WITH_EDITORONLY_DATA
	SceneComponent->bVisualizeComponent = true;
#endif

	RootComponent = SceneComponent;

	// Property Initialize
	EntryMoveSpeed = 400;
	Width = 400;
	Height = 300;

	SetReplicates(true);
}
	
// Called when the game starts or when spawned
void ATimelineCommentManager::BeginPlay()
{
	UWorld* CurWorld = GetWorld();
	TSubclassOf<AActor> ActorClass = ADirectorCameraActor::StaticClass();
	if (ActorClass && CurWorld)
	{
		for (TActorIterator<AActor> It(CurWorld, ActorClass); It; ++It)
		{
			ADirectorCameraActor* DirectorCamera = Cast<ADirectorCameraActor>(*It);
			if (!DirectorCamera->IsPendingKill())
			{
				// make time line manager on the director camera normal, then attach
				FVector NewLocation = UKismetMathLibrary::ProjectPointOnToPlane(
					DirectorCamera->GetActorLocation(), GetActorLocation(), DirectorCamera->GetActorForwardVector());
				SetActorLocation(NewLocation);
				AttachToActor(DirectorCamera, FAttachmentTransformRules::KeepWorldTransform);

				// calc width and height
				float Distance = (NewLocation - DirectorCamera->GetActorLocation()).Size();
				UCameraComponent* CameraComponent = DirectorCamera->GetCameraComponent();
				Width = UKismetMathLibrary::DegTan(CameraComponent->FieldOfView / 2) * 2 * Distance;
				Height = Width / CameraComponent->AspectRatio;
			}
		}
	}

	Super::BeginPlay();
}

// Called every frame
void ATimelineCommentManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bDrawDebugData) 
	{
		DrawDebugData(GetWorld());
	}

	float EntryTickMoveSpeed = EntryMoveSpeed * DeltaTime;
	for (int32 Index = 0; Index < TimelineCommentEntrys.Num(); Index++)
	{
		ATimelineCommentEntry* Entry = TimelineCommentEntrys[Index];
		FIntPoint Bounds = Entry->GetCurrentDrawSize();

		if (Entry->GetActorLocation().Y - Bounds.X > Width / 2)
		{
			TimelineCommentEntrys[Index]->Destroy();
			TimelineCommentEntrys.RemoveAtSwap(Index);
			Index--;
		}
		else
		{
			Entry->AddActorLocalOffset(FVector(0, EntryTickMoveSpeed, 0));
		}
	}
}

void ATimelineCommentManager::BroadcastVoiceText_Implementation(const FString& Value)
{
	UWorld* CurWorld = GetWorld();
	if (CurWorld)
	{
		ATimelineCommentEntry* Entry = CurWorld->SpawnActor<ATimelineCommentEntry>(
			FVector(0.f, -Width / 2, Height / 4), FRotator::ZeroRotator);
		Entry->SetText(FText::FromString(Value));
		Entry->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		TimelineCommentEntrys.Add(Entry);
	}
}

void ATimelineCommentManager::DrawDebugData(UWorld* InWorld, bool bPersistant, float lifeTime)
{
	FVector CenterPosition = SceneComponent->K2_GetComponentLocation();
	FVector TopLeftPoint = CenterPosition + FVector(0.f, -Width / 2, Height / 2);
	FVector TopRightPoint = CenterPosition + FVector(0.f, Width / 2, Height / 2);
	FVector BottomLeftPoint = CenterPosition + FVector(0.f, -Width / 2, -Height / 2);
	FVector BottomRightPoint = CenterPosition + FVector(0.f, Width / 2, -Height / 2);

	DrawDebugLine(InWorld, TopLeftPoint, TopRightPoint, FColor(255, 128, 0), bPersistant, lifeTime, 0, 20);
	DrawDebugLine(InWorld, TopRightPoint, BottomRightPoint, FColor(255, 128, 0), bPersistant, lifeTime, 0, 20);
	DrawDebugLine(InWorld, BottomRightPoint, BottomLeftPoint, FColor(255, 128, 0), bPersistant, lifeTime, 0, 20);
	DrawDebugLine(InWorld, BottomLeftPoint, TopLeftPoint, FColor(255, 128, 0), bPersistant, lifeTime, 0, 20);
}

