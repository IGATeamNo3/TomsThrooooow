// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "TimelineCommentEntry.generated.h"

UCLASS()
class TOMSTHROOOOOW_API UMyWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	UMyWidgetComponent(const FObjectInitializer& ObjectInitializer);

	FIntPoint GetCurrentDrawSize() { return CurrentDrawSize; }
};

UCLASS(NotPlaceable)
class TOMSTHROOOOOW_API ATimelineCommentEntry : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimelineCommentEntry(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetText(const FText& Value);

	FORCEINLINE FIntPoint GetCurrentDrawSize() const
	{
		return Widget->GetCurrentDrawSize();
	}
private:
	class UTextRenderComponent* TextRender;

	class UMyWidgetComponent* Widget;
};
