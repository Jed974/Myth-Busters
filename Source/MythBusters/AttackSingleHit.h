// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

extern class AHitBoxGroup;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackSingleHit.generated.h"



UCLASS()
class MYTHBUSTERS_API UAttackSingleHit : public UAttack
{
	GENERATED_BODY()

public :
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroup> hitBoxGroupToSpawn;
	UPROPERTY(Category = "HitCreation", EditAnywhere, BlueprintReadWrite)
		FName SocketToAttachTo;

	UPROPERTY(Category = "Track", VisibleAnywhere, BlueprintReadWrite)
		AHitBoxGroup* hitBoxGroup;



public:
	UAttackSingleHit();
	virtual void StopAttack() override;

	UFUNCTION(BlueprintCallable)
	void SpwanHitBoxGroup();
	UFUNCTION(BlueprintCallable)
	void DestroyHitBoxGroup();

	virtual void ApplySaveState(UAttackSaveState _saveState) override;

	virtual void OnActiveNotify() override;
	virtual void OnInactiveNotify() override;
};
