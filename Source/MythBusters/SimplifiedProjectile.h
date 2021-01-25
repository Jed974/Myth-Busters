// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AHitBoxGroupProjectile;
#include "CoreMinimal.h"
#include "SimplifiedProjectile.generated.h"


USTRUCT(BlueprintType)
struct FSimplifiedProjectileArray{
	GENERATED_BODY()

	TArray<FSimplifiedProjectile> Projectiles;
};

USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FSimplifiedProjectile
{
	GENERATED_BODY()

	bool facingRight;
	int lifeTime;
	FTransform transform;
	int auxiliaryInfo;

	FSimplifiedProjectile();
	FSimplifiedProjectile(AHitBoxGroupProjectile* model);
	~FSimplifiedProjectile();
};
