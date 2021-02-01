// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AHitBoxGroupProjectile;
#include "CoreMinimal.h"
#include "SimplifiedProjectile.generated.h"

/// Struct containing an array of FSimplifiedProjectile, usefull for rollbacks
USTRUCT(BlueprintType)
struct FSimplifiedProjectileArray{
	GENERATED_BODY()

	TArray<FSimplifiedProjectile> Projectiles;
};

/// Simplified version of AHitBoxGroupProjectile, used for save state of AHitBoxGroupProjectile
USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FSimplifiedProjectile
{
	GENERATED_BODY()

	/// The original projectile was facing right
	bool facingRight;
	/// The original projectile had already hit
	bool alreadyHit;
	/// The original projectile's lifetime
	int lifeTime;
	/// The original projectile's transform
	FTransform transform;
	/// The original projectile's auxiliary information
	int auxiliaryInfo;

	FSimplifiedProjectile();
	/// Constructs a FSimplifiedProjectile directly from a AHitBoxGroupProjectile
	FSimplifiedProjectile(AHitBoxGroupProjectile* model);
	~FSimplifiedProjectile();
};
