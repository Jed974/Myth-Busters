// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "god.h"
#include "GameFramework/Actor.h"
#include "HitBoxGroup.generated.h"

UCLASS()
class MYTHBUSTERS_API AHitBoxGroup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AGod*> AlreadyHitGods;
	
public:	
	// Sets default values for this actor's properties
	AHitBoxGroup();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
