// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitBoxGroup.h"
#include "HitBoxGroupProjectile.generated.h"


USTRUCT()
struct FSimplifiedProjectile {
	GENERATED_BODY()

	//UPROPERTY(Category = "Attack", VisibleAnywhere)
	bool facingRight;
	float lifeTime;
	FTransform transform;

	FSimplifiedProjectile();
	FSimplifiedProjectile(AHitBoxGroupProjectile* model);
};


/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API AHitBoxGroupProjectile : public AHitBoxGroup
{
	GENERATED_BODY()

public:
	/** Delay before the group is deleted */
	UPROPERTY(Category = "Projectile Parameters", EditAnywhere, BlueprintReadWrite)
		float LifeTime;

	/** Movement Speed */
	UPROPERTY(Category = "Projectile Parameters", EditAnywhere, BlueprintReadWrite)
		FVector Speed;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FSimplifiedProjectile getSimplifiedVersion();
	void applySimplifiedVersion(const FSimplifiedProjectile& simple);
};
