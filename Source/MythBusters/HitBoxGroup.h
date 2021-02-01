// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "god.h"
#include "GameFramework/Actor.h"
#include "HitBoxGroup.generated.h"

UCLASS()
class MYTHBUSTERS_API AHitBoxGroup : public AActor
{
	GENERATED_BODY()

		// === At first there was an array of hit gods but we changed it to a bool as the is only one other player ===
/*protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AGod*> AlreadyHitGods;*/
	
public:	
	// Sets default values for this actor's properties
	AHitBoxGroup();
	/// Indicates if the god instigator was facing right when the hitBoxGroup was created
	UPROPERTY(Category = "Orientation", BlueprintReadWrite)
	bool facingRight;

	/// Indicates if the hitBoxGroup has already hit a god
	bool alreadyHit = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// Originally returns if the specified god has been hurted ; now only returns if a god has already been hit and if godHit isn't the instigator
	bool GodHitIsValid(AGod* godHit);
	//void RegisterGodHit(AGod* godHit);
};
