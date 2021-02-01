// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attack.h"
#include "GodBoostComponent.h"
#include "AttackBoost.generated.h"

/**
 * Attack triggering a boost on one of its owner's statistics (Speed, DashSpeed, Shield, Damage, EjectionForce)
 */
UCLASS()
class MYTHBUSTERS_API UAttackBoost : public UAttack
{
	GENERATED_BODY()
	
public :
	/// Boosting percent, can be negative ; 0 is no effect
	UPROPERTY(Category = "Boost", EditAnywhere, BlueprintReadWrite)
		int boostPercent = 0;
	/// Number of frame during which boost will be active
	UPROPERTY(Category = "Boost", EditAnywhere, BlueprintReadWrite)
		int frameDuration = 0;
	/// Type of boost applies (Attack, speed, shield...)
	UPROPERTY(Category = "Boost", EditAnywhere, BlueprintReadWrite)
		EBoostType boostType;

public :
	UAttackBoost();

	/// ApplyBoost to the attack's god owner
	void ApplyBoost();

	virtual void OnActiveNotify() override;
};
