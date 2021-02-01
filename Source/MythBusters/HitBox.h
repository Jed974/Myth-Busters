// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HitBox.generated.h"

/**
 * Components of HitBoxGroups that are holding the information of how to hurt and ejected the other player if it collides
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTHBUSTERS_API UHitBox : public USphereComponent
{
	GENERATED_BODY()

public:
	/// Priority of the hitbox : in case several hitboxs are hit at the same frame, hte one with the highest priority is used
	UPROPERTY(Category = "Default", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int Priority = 0;
	/// Damages done by the hitBox
	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float BaseDamage;
	/// Angle at which the hitBox ejects the other player ; goes from 0 to 360 in trigonometric sens and 0 is toward positive X axis
	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float AngleDeg;
	/// Speed at which the hitBox ejects the other player regardless the damages he has accumulated;
	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float BaseKnockBack;
};
