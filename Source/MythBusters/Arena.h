// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaElement.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

UCLASS()
class MYTHBUSTERS_API AArena : public AActor
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties
	AArena();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		TArray<UArenaElement*> Grounds;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoliageRoot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		float MinimalSpeedToDie = 500.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsEjected(float _velocityNorm, FHitResult& _hitResult);
	UFUNCTION(BlueprintImplementableEvent)
		void EGodEjected();
};
