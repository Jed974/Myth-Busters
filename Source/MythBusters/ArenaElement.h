// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ArenaElement.generated.h"

/**
 * Element composing the circular ground of an arena
 */
UCLASS()
class MYTHBUSTERS_API UArenaElement : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	/// identifies the ArneaElement among the arena's arenaElements
	UPROPERTY(BlueprintReadWrite)
		int IdElement;

public:
	/// Sets ArenaElement's id
	virtual void SetId(int i);
	/// Gets ArenaElement's id
	virtual int GetId();
	/// Perform the ArenaElement's explosion because of a god breaking the arena at this ArenaElement (simply turns visibility off)
	virtual void Explode();
};
