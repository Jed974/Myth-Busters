// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaElement.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

/**
* Arena Actor, composed of several ArenaElements that creates a circular wall around the players.
* If a god is ejected with enough speed against one of the Arena's elements it will break it, otherwise the player will bounce of that wall.
*/
UCLASS()
class MYTHBUSTERS_API AArena : public AActor
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties
	AArena();

	/// Component parent of all the other scene components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;

	/// Physical ground of the arena, composed of several (36) ArenaElement that creates a circular ground
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		TArray<UArenaElement*> Grounds;

	/// Component parent of all the foliage meshs (physic collisions should de disabled on those
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoliageRoot;

	/// The minimal speed that an ejected god should have when colliding with the arena to break the arena and end the fight
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Ejection", meta = (AllowPrivateAccess = "true"))
		float MinimalSpeedToDie = 5000.0f;

	/// Actor to spawn when a god is breaking the arena. Its size should be 3 arenaElements
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Ejection", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> explodeActor;

protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Check if the speed given has enough speed to break the arena or to bounce on it
	bool IsEjected(float _velocityNorm, FHitResult& _hitResult);
	
	/// Event triggered when a god is definitely ejected
	UFUNCTION(BlueprintImplementableEvent)
	void EGodEjected();
};
