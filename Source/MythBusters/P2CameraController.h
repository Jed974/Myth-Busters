// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P2CameraController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UP2CameraController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP2CameraController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, CATEGORY = "Players")
	AActor* Player1;

	UPROPERTY(EditAnywhere, CATEGORY = "Players")
	AActor* Player2;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
	float initialCameraDistance;

};
