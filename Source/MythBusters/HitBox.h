// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HitBox.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTHBUSTERS_API UHitBox : public USphereComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Default", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int Priority = 0;
	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float BaseDamage;
	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float AngleDeg;
	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float BaseKnockBack;
};
