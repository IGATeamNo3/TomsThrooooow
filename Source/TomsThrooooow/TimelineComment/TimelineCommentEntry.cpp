// Fill out your copyright notice in the Description page of Project Settings.

#include "TomsThrooooow.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TimelineCommentEntry.h"


UMyWidgetComponent::UMyWidgetComponent(const FObjectInitializer& ObjectInitializer)
{
	bDrawAtDesiredSize = true;
}

// Sets default values
ATimelineCommentEntry::ATimelineCommentEntry(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set widget
	Widget = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("Widget"));
	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/TomsThrooooow/Blueprints/UI/UMG_TimelineCommentEntry.UMG_TimelineCommentEntry_C'"));
	Widget->SetWidgetClass(WidgetClass);
	Widget->SetPivot(FVector2D(0,0.5f));

	RootComponent = Widget;
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

	FIntPoint DrawSize = Widget->GetCurrentDrawSize();
	DrawDebugBox(GetWorld(), Widget->K2_GetComponentLocation() + FVector(0, -DrawSize.X / 2 , 0), FVector(0.f, DrawSize.X / 2, DrawSize.Y / 2), FColor(255, 128, 0), false, -1, 0, 10);
}

void ATimelineCommentEntry::SetText(const FText& Value)
{
	UUserWidget* UserWidget = Widget->GetUserWidgetObject();
	if (!UserWidget)
	{
		return;
	}

	UTextBlock* TimelineCommentTextWidget = Cast<UTextBlock>(UserWidget->GetWidgetFromName(FName("TimelineCommentText")));
	if (!TimelineCommentTextWidget)
	{
		return;
	}

	TimelineCommentTextWidget->SetText(Value);
}

