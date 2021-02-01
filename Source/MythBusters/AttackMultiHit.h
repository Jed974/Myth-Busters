// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

extern class AHitBoxGroup;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackMultiHit.generated.h"

/**
 * Attack during which several HitboxGroups are created sequentially.
 * The animNotify to use are :
 * - AttackAnimActive
 * - AttackAnimChange (as many times as you have hitBoxGroups to spwan)
 * - AttackAnimInactive
 */
UCLASS()
class MYTHBUSTERS_API UAttackMultiHit : public UAttack
{
	GENERATED_BODY()

private:
	/// HitBoxGroup to spawn next
	int idHitboxGroup;

public:
	/// Array containing classes of the HitBoxGroups to spawn
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AHitBoxGroup>> hitBoxGroupsToSpawn;
	/// Socket to which the HitBoxGroups created must be attached
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		FName SocketToSpawnTo;

	/// Current hitBoxGroup spawned
	UPROPERTY(Category = "Track", VisibleAnywhere, BlueprintReadWrite)
		AHitBoxGroup* hitBoxGroup;



public:
	UAttackMultiHit();
	virtual void StopAttack() override;

	/// Spawns the nextHitBoxGroup to spawn ; alreadyHit is used to set if the hitBoxGroup has already hit a god (rollback purposes)
	UFUNCTION(BlueprintCallable)
		void SpawnHitBoxGroup(int id, bool alreadyHit = false);
	/// Destroys current HitBoxGroup
	UFUNCTION(BlueprintCallable)
		void DestroyHitBoxGroup();

	virtual void ApplySaveState(UAttackSaveState _saveState, bool _playAnimation = true) override;

	virtual void OnActiveNotify() override;
	virtual void OnChangeNotify() override;
	virtual void OnInactiveNotify() override;
};
