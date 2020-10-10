// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
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

	/** The extra time (in frames) it takes to start flying horizontally behind the character  */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int HorizontalTurnaroundTime;

	/** The extra time (in frames) it takes to turn around when flying horizontally  */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int HorizontalFlyTurnaroundTime;

	/** The maximum speed when flying. 1: Lateral speed; 2: Upward speed; 3: Downward speed; */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxVerticalFlySpeed;

	/** The time (in frames) it takes to reach inputted vertical fly speed */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalFlyStartupTime;

	/** The time (in frames) it takes to decrease speed to 0 when flying vertically */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalFlyStopTime;

	/** The extra time (in frames) it takes to start flying vertically in the opposite direction of current movement  */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalTurnaroundTime;

	/** The extra time (in frames) it takes to change direction when flying vertically  */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalFlyTurnaroundTime;

	enum EHorizontalMovementState
	{
		HorizontalNeutral,
		HorizontalTurnAround,
		FlyHorizontalStartup,
		FlyHorizontal,
		FlyHorizontalTurnAround,
		FlyHorizontalStop,
		SprintLeftStartup,
		SprintLeft,
		SprintRightStartup,
		SprintRight,
		SprintTurnAroundLeft,
		SprintTurnaroundRight,
		HorizontalSprintStop
	};

	enum EVerticalMovementState
	{
		VerticalNeutral,
		VerticalTurnAround,
		FlyVerticalStartup,
		FlyVertical,
		FlyVerticalTurnAround,
		FlyVerticalStop,
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

	UPROPERTY(BlueprintReadOnly)
	bool isFacingRight;
	UPROPERTY(BlueprintReadOnly)
	bool isFacingUp;

	UCapsuleComponent* CapsuleComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void ChangeHorizontalMovementState(EHorizontalMovementState NewState);

	virtual void ChangeVerticalMovementState(EVerticalMovementState NewState);



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void AddMovementInput(FVector2D Direction, float amount);

	virtual void ComputeNewLocation(FVector & Location);
		
};
