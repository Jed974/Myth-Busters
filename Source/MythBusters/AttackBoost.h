// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack.h"
#include "GodBoostComponent.h"
#include "AttackBoost.generated.h"

/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UAttackBoost : public UAttack
{
	GENERATED_BODY()
	
public :
	UPROPERTY(Category = "Boost", EditAnywhere, BlueprintReadWrite)
		int boostPercent = 0;
	UPROPERTY(Category = "Boost", EditAnywhere, BlueprintReadWrite)
		int frameDuration = 0;
	UPROPERTY(Category = "Boost", EditAnywhere, BlueprintReadWrite)
		EBoostType boostType;

public :
	UAttackBoost();
	virtual void StartAttack() override;
	virtual void StopAttack() override;

	void ApplyBoost();

	virtual void OnActiveNotify() override;
};
