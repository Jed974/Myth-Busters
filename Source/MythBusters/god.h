// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GodMovementComponent.h"
#include "GameFramework/Actor.h"
#include "god.generated.h"

UCLASS()
class MYTHBUSTERS_API Agod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Agod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, CATEGORY = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly, CATEGORY = "Movement")
	UGodMovementComponent* GodMovement;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
