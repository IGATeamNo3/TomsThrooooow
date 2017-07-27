// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TimelineCommentManager.generated.h"

class ATimelineCommentEntry;

UCLASS()
class TOMSTHROOOOOW_API ATimelineCommentManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimelineCommentManager(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = TimelineCommentManager)
	void AddVoiceText(const FString& Value);

private:
	void DrawDebugData(UWorld* InWorld, bool bPersistant = false, float lifeTime = -1);

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, Category = TimelineCommentManager)
	bool bDrawDebugData;

	UPROPERTY(EditAnywhere, Category = TimelineCommentManager)
	float EntryMoveSpeed;

	float Width;
	float Height;

	TArray<ATimelineCommentEntry*> TimelineCommentEntrys;
};
