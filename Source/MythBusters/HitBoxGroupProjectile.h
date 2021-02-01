// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitBoxGroup.h"
#include "SimplifiedProjectile.h"
#include "HitBoxGroupProjectile.generated.h"



/**
 * HitBoxGroup that isn't attached to the player. It moves at a specific speed and dies automatically after some times
 */
UCLASS()
class MYTHBUSTERS_API AHitBoxGroupProjectile : public AHitBoxGroup
{
	GENERATED_BODY()

public:
	int auxiliaryInfo = -1;
	/** Delay before the group is deleted */
	UPROPERTY(Category = "Projectile Parameters", EditAnywhere, BlueprintReadWrite)
		int LifeTime;

	/** Movement Speed */
	UPROPERTY(Category = "Projectile Parameters", EditAnywhere, BlueprintReadWrite)
		FVector Speed;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// Return a simplified version of the projectile (for save state)
	UFUNCTION(BlueprintCallable)
	FSimplifiedProjectile getSimplifiedVersion();
	/// Apply a simplified version of the projectile (for load state)
	UFUNCTION(BlueprintCallable)
	void applySimplifiedVersion(const FSimplifiedProjectile& simple);
};
