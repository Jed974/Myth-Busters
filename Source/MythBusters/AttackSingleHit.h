// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

extern class AHitBoxGroup;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackSingleHit.generated.h"


/**
* Attack creating a single HitBoxGroup attached to the god
*/
UCLASS()
class MYTHBUSTERS_API UAttackSingleHit : public UAttack
{
	GENERATED_BODY()

public :
	/// Class of the HitBoxGroup to spawn
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroup> hitBoxGroupToSpawn;
	/// Socket to which attach the hitBoxGroup
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		FName SocketToSpawnTo;

	/// Pointer pointing to the hitBoxGroup spawned
	UPROPERTY(Category = "Track", VisibleAnywhere, BlueprintReadWrite)
		AHitBoxGroup* hitBoxGroup;



public:
	UAttackSingleHit();
	virtual void StopAttack() override;

	/// Spawns the hitboxGroup and attachs it to the socket
	UFUNCTION(BlueprintCallable)
	void SpawnHitBoxGroup(bool alreadyHit = false);
	/// Destroys the hitBoxGroup spawned
	UFUNCTION(BlueprintCallable)
	void DestroyHitBoxGroup();

	virtual void ApplySaveState(UAttackSaveState _saveState, bool _playAnimation = true) override;

	virtual void OnActiveNotify() override;
	virtual void OnInactiveNotify() override;
};
