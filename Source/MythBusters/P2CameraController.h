// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "god.h"
#include "Components/ActorComponent.h"
#include "P2CameraController.generated.h"

/**
* Special type of camera that always keep track on screen of the gods present in the scene
*/
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

	/// The gods to keep on screen
	TArray<AActor*> Gods;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// The initoal camera distance from the players
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
	float initialCameraDistance = 600;

	/// Find the gods present in the scene
	UFUNCTION(BlueprintCallable)
	void FindGods();
};
