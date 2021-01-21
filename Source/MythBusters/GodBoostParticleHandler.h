// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AGod;
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GodBoostParticleHandler.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYTHBUSTERS_API UGodBoostParticleHandler : public UObject
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AGod* godOwner;


	UFUNCTION(BlueprintImplementableEvent)
	void SpawnParticleSystem(int boostId, bool boosting);
	UFUNCTION(BlueprintImplementableEvent)
	void DeleteParticleSystem(int boostId);
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitArray();
	void SetUpGod(AActor* owner);
};
