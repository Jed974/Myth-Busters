// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

extern class AHitBoxGroupProjectile;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackProjectile.generated.h"

/**
 * Attack spawning a projectiles that evolves on its own during a specified lifetime
 */
UCLASS()
class MYTHBUSTERS_API UAttackProjectile : public UAttack//, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	/// HitBoxGroupProjectile to spawn
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroupProjectile> projectileToSpawn;

	/// Socket to spawn the projectile to
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		FName SocketToSpawnTo;


public:
	UAttackProjectile();

	/// Spawns the projectile
	UFUNCTION(BlueprintCallable)
	AHitBoxGroupProjectile* SpawnHitBoxGroup();

	virtual void OnActiveNotify() override;

	/*
	* === Methods needed to make an attack a TickableGameObject ===
	*			eventually useless for AttackProjectile
	*			but may be needed for futur attack types
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;
	*/
};
