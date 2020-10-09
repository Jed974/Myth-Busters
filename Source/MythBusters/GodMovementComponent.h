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
	/** The maximum speed when flying. */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxHorizontalFlySpeed;


	/** The time (in frames) it takes to reach inputted horizontal fly speed */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int HorizontalFlyStartupTime;
	
	/** The time (in frames) it takes to decrease speed to 0 when flying horizontally */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int HorizontalFlyStopTime;

	/** The maximum speed when flying. 1: Lateral speed; 2: Upward speed; 3: Downward speed; */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxVerticalFlySpeed;

	/** The time (in frames) it takes to reach inputted vertical fly speed */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalFlyStartupTime;

	/** The time (in frames) it takes to decrease speed to 0 when flying vertically */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalFlyStopTime;

	enum EHorizontalMovementState
	{
		HorizontalNeutral,
		HorizontalTurnaround,
		FlyLeftStartup,
		FlyLeft,
		FlyRightStartup,
		FlyRight,
		FlyLeftTurnaround,
		FlyRightTurnaround,
		FlyLeftStop,
		FlyRightStop,
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
		FlyDownTurnAround,
		FlyUpTurnAround,
		FlyDownStop,
		FlyUpStop,
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

	float HorizontalSpeed;
	int CurrentHorizontalStateTimer;
	
	int CurrentVerticalStateTimer;
	float VerticalSpeed;
	float DELTA_TIME;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void ChangeHorizontalMovementState(EHorizontalMovementState NewState);

	virtual void ChangeVerticalMovementState(EVerticalMovementState NewState);



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void AddMovementInput(FVector2D Direction, float amount);
		
};
