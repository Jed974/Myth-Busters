// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GodMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UGodMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodMovementComponent();

public:
	/** The maximum speed when flying. 1: Lateral speed; 2: Upward speed; 3: Downward speed; */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxHorizontalFlySpeed;

	/** The maximum speed when flying. 1: Lateral speed; 2: Upward speed; 3: Downward speed; */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxVerticalFlySpeed;

	enum EHorizontalMovementState
	{
		HorizontalNeutral,
		FlyLeftStartup,
		FlyLeft,
		FlyRightStartup,
		FlyRight,
		FlyTurnAroundLeft,
		FlyTurnAroundRight,
		HorizontalFlyStop,
		SprintLeftStartup,
		SprintLeft,
		SprintRightStartup,
		SprintRight,
		SprintTurnaroundLeft,
		SprintTurnaroundRight,
		HorizontalSprintStop
	};

	enum EVerticalMovementState
	{
		VerticalNeutral,
		FlyDownStartup,
		FlyDown,
		FlyUpStartup,
		FlyUp,
		FlyTurnAroundDown,
		FlyTurnAroundUp,
		VerticalFlyStop,
		SprintDownStartup,
		SprintDown,
		SprintUpStartup,
		SprintUp,
		SprintTurnaroundDown,
		SprintTurnaroundUp,
		VerticalSprintStop
	};

	EHorizontalMovementState HorizontalMovementState;
	EVerticalMovementState VerticalMovementState;

protected:
	FVector2D _MovementInput;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void AddMovementInput(FVector2D Direction, float amount);
		
};
