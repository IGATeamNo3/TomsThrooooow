// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TimelineCommentEntry.generated.h"

UCLASS(/*NotPlaceable*/)
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

	FORCEINLINE void SetText(const FText& Value)
	{
		TextRender->SetText(Value);
	}

	FORCEINLINE FBoxSphereBounds CalcBounds() const
	{
		return TextRender->CalcBounds(GetActorTransform());
	}
private:
	class UTextRenderComponent* TextRender;
	//class UWidgetComponent* Widget;
};
