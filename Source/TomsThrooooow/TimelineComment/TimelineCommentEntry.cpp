// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "TimelineCommentEntry.h"


// Sets default values
ATimelineCommentEntry::ATimelineCommentEntry(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NewTextRenderComponent"));
	TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRender->SetWorldSize(60);

	RootComponent = TextRender;
}

// Called when the game starts or when spawned
void ATimelineCommentEntry::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATimelineCommentEntry::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FBoxSphereBounds Bounds = TextRender->CalcBounds(GetActorTransform());
	DrawDebugBox(GetWorld(), Bounds.GetBox().GetCenter(), Bounds.GetBox().GetExtent(), FColor(255, 128, 0), false, -1, 0, 20);
}

