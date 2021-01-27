// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBoostParticleHandler.h"
#include "Components/ActorComponent.h"
#include "GodBoostComponent.generated.h"


UENUM(BlueprintType)
enum class EBoostType : uint8 {
	SPEED,
	DASH,
	SHIELD,
	ATTACK,
	EJECTION,
};
USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FBoost {
	GENERATED_BODY()

	int frameAlive;
	int tempPercent;
	int definitivePercent;

	FBoost();
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UGodBoostComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodBoostComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Boosts")
	TArray<FBoost> boosts;
	UPROPERTY(EditAnywhere, Category = "Boosts")
	TSubclassOf<UGodBoostParticleHandler> particleHandlerClass;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Boosts")
	UGodBoostParticleHandler* particleHandler;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TemporaryBoost(EBoostType type, int percent, int durationFrames);
	UFUNCTION(BlueprintCallable)
	void GameBoost(EBoostType type, int percent);

	UFUNCTION(BlueprintCallable)
	float GetBoost(EBoostType type);

	TArray<FBoost> SaveBoostState() const;
	void LoadBoostState(TArray<FBoost> &savedBoosts);

};
