// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AGod;
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GodBoostParticleHandler.generated.h"

/**
 * Class to deal with the particules representing the boosts status of a god.
 * Use the blueprint derivated class for all gods as it has the logic and
 * particules systems integrated.
 */
UCLASS(Blueprintable)
class MYTHBUSTERS_API UGodBoostParticleHandler : public UObject
{
	GENERATED_BODY()

public :
	/// The owner 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AGod* godOwner;

	/// Spawns a particle system based on the boost type (converted to int) and if the boost is positive or negative (boosting)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Particle")
	void SpawnParticleSystem(int boostId, bool boosting);
	/// Delete the particle system spawned for the boost type because the effect is over
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Particle")
	void DeleteParticleSystem(int boostId);
	
	/// Initialize the particles systems array (must be implemented in blueprint)
	UFUNCTION(BlueprintImplementableEvent)
	void InitArray();
	/// Registers its owner as a god pointer
	void SetUpGod(AActor* owner);
};
