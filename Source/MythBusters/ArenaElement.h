// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ArenaElement.generated.h"

/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UArenaElement : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
		int IdElement;

public:
	virtual void SetId(int i);
	virtual int GetId();
	virtual void Explode();
};
