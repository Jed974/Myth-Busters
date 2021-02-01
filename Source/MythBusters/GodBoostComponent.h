// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBoostParticleHandler.h"
#include "Components/ActorComponent.h"
#include "GodBoostComponent.generated.h"


/// Type of Boost
UENUM(BlueprintType)
enum class EBoostType : uint8 {
	SPEED,
	DASH,
	SHIELD,
	ATTACK,
	EJECTION,
};
/// Boost class ; composed of a temporary boost and a permanent boost
USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FBoost {
	GENERATED_BODY()

	/// How many frames the temporary boost will still be active
	int frameAlive;
	/// Temporary boost percent
	int tempPercent;
	/// permanent boost percent
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

	/// Array of all god boosts (5, one for each EBoostType)
	UPROPERTY(VisibleAnywhere, Category = "Boosts")
	TArray<FBoost> boosts;
	/// Particule handler to spawn to deal with to have a visual representation of the boosts. If nullptr, the game will crash
	UPROPERTY(EditAnywhere, Category = "Boosts")
	TSubclassOf<UGodBoostParticleHandler> particleHandlerClass;

	/// Pointer of the particleHandler created from particleHandlerClass
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Boosts")
	UGodBoostParticleHandler* particleHandler;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// Adds a temporary boost
	UFUNCTION(BlueprintCallable)
	void TemporaryBoost(EBoostType type, int percent, int durationFrames);
	/// Adds a permanent boost
	UFUNCTION(BlueprintCallable)
	void GameBoost(EBoostType type, int percent);

	/// Get the boost value (based on temporary and permanent boost)
	UFUNCTION(BlueprintCallable)
	float GetBoost(EBoostType type);

	/// Performs a Save State of all god's boosts
	TArray<FBoost> SaveBoostState() const;
	/// Loads the save Sate of god's boosts
	void LoadBoostState(TArray<FBoost> &savedBoosts);

};
