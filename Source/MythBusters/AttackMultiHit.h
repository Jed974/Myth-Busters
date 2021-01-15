// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

extern class AHitBoxGroup;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackMultiHit.generated.h"

/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UAttackMultiHit : public UAttack
{
	GENERATED_BODY()

private:
	int idHitboxGroup;

public:
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AHitBoxGroup>> hitBoxGroupsToSpawn;
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		FName SocketToAttachTo;

	UPROPERTY(Category = "Track", VisibleAnywhere, BlueprintReadWrite)
		AHitBoxGroup* hitBoxGroup;



public:
	UAttackMultiHit();
	virtual void StartAttack() override;
	virtual void StopAttack() override;

	UFUNCTION(BlueprintCallable)
		void SpwanHitBoxGroup(int id);
	UFUNCTION(BlueprintCallable)
		void DestroyHitBoxGroup();

	virtual void OnActiveNotify() override;
	virtual void OnChangeNotify() override;
	virtual void OnInactiveNotify() override;
};
