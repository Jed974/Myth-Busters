// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

extern class AHitBoxGroupProjectile;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UAttackProjectile : public UAttack//, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroupProjectile> projectileToSpawn;
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		FName SocketToAttachTo;

	//UPROPERTY(Category = "Track", VisibleAnywhere, BlueprintReadWrite)
	//	TArray<AHitBoxGroupProjectile*> Projectiles;



public:
	UAttackProjectile();
	virtual void StartAttack() override;
	virtual void StopAttack() override;

	UFUNCTION(BlueprintCallable)
	AHitBoxGroupProjectile* SpwanHitBoxGroup();
	//UFUNCTION(BlueprintCallable)
		//void DestroyAllProjectiles();

	virtual void OnActiveNotify() override;

	/*
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;
	*/
};
