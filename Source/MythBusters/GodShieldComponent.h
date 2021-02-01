// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class AGod;
#include "Shield.h"
#include "Components/ActorComponent.h"
#include "GodShieldComponent.generated.h"



/// SaveState of the shield
USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FShieldSaveState {
	GENERATED_BODY()

	/// Is the shield out
	bool out;
	/// what is the lifetime (=energy) of the shield
	int lifeTime;
	/// toward which angle is looking the shield
	float angle;
};

/**
* Component managing the god's shield
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UGodShieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodShieldComponent();

protected:
	/// Component's owner as shield
	AGod* godOwner;

	/// Shield class to spawn, always use the blueprint derivated BP_SHield ; if null game will crash when a player will try to shield
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AShield> ShieldClassToSpwan;
	/// Shield's fresnel color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldFresnelColor = FColor::Blue;
	/// Shield's base color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldBaseColor = FColor::Red;
	/// Shield's size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		float ShieldSize = 1.8f;

	/// Pointer to the current shield spawned
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Shield")
		AShield* CurrentShield = nullptr;

	/// Constant : a frame duration 
	float DELTA_TIME;
	/// Life time of a shield -> influences its range ; it's only copied when the shield is destroyed and it slowly decrease back to 0 (= 360 shield range)
	float shieldLifeTime = 0;

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// Set colors to fit if the character is the player 1 or 2
	void SetNoneCustomColors(int idPlayer);

	/// Spawn a shield
	bool StartShield();
	/// Destroy the shield
	void StopShield();

	/// Gives the X orientation of the shield
	void OrientShieldX(float AxisValue);
	/// Gives the Y orientation of the shield
	void OrientShieldY(float AxisValue);

	/// Saves the shield save state
	UFUNCTION(BlueprintCallable)
	FShieldSaveState SaveShieldState();
	/// Loads the shield save state
	UFUNCTION(BlueprintCallable)
	void LoadShieldState(FShieldSaveState &ShSvSt);
};
