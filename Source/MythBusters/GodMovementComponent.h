// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Arena.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GodMovementComponent.generated.h"


DECLARE_DELEGATE_OneParam(FChangeMovementStateDelegate, EMovementState);
DECLARE_DELEGATE(FInstantTurnDelegate);

UENUM(BlueprintType)
enum class EMovementState : uint8 {
	Flying,
	FlyingTurnaroud,
	Dashing,
	Sprinting,
	Ejected,
	WallHit,
	DeathEjected
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UGodMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UGodMovementComponent();

public:

	FChangeMovementStateDelegate ChangeMovementStateDelegate;
	FInstantTurnDelegate InstantTurnDelegate;

	UPROPERTY(BlueprintReadOnly)
		EMovementState MovementState;
	
	/** The maximum speed when flying. */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxHorizontalFlySpeed;

	/** The startup speed when initiating a flying movement. */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float StartupHorizontalFlySpeed;

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

	/** The startup speed when initiating a flying movement. */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float StartupVerticalFlySpeed;

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


	/** The time it takes to get control over your character again when being ejected. */
	UPROPERTY(Category = "God Movement: Ejection", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float EjectionRecoverTime;

	/** Number of frames a God stays on a wall while ejected */
	UPROPERTY(Category = "God Movement: Ejection", VisibleAnywhere, meta = (ClampMin = "0", UIMin = "0"))
		int WallHitFrames = 12;

	/** Dashing speed, as a multiplier of maximum flying speed */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float DashingSpeedScale;

	/** Number of frames in a dash */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
		int DashFrames;

	/** Dashing speed, as a multiplier of maximum flying speed */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float SprintSpeedScale;

	enum EHorizontalMovementState
	{
		HorizontalNeutral,
		HorizontalTurnAround,
		FlyHorizontalStartup,
		FlyHorizontal,
		FlyHorizontalTurnAround,
		FlyHorizontalStop,
		SprintHorizontalStartup,
		SprintHorizontal,
		SprintHorizontalTurnAround,
		SprintHorizontalStop,
	};

	enum EVerticalMovementState
	{
		VerticalNeutral,
		VerticalTurnAround,
		FlyVerticalStartup,
		FlyVertical,
		FlyVerticalTurnAround,
		FlyVerticalStop,
		SprintVerticalStartup,
		SprintVertical,
		SprintVerticalTurnaround,
		SprintVerticalStop,
	};

	EHorizontalMovementState HorizontalMovementState;
	EVerticalMovementState VerticalMovementState;

	UPROPERTY(BlueprintReadOnly)
	FVector2D Velocity;
	FVector2D PushVelocity;
	UPROPERTY(BlueprintReadOnly)
	bool IsPushable;
	FVector2D EjectionVelocity;


protected:
	
	FVector2D _MovementInput;

	float HorizontalSpeed;
	float HorizontalPreviousSpeed;
	int CurrentHorizontalStateTimer;
	
	int CurrentVerticalStateTimer;
	float VerticalSpeed;
	float VerticalPreviousSpeed;

	int DashFrameCounter = 0;
	int EjectionFrameCounter = 0;
	int WallHitFrameCounter = 0;
	
	float DELTA_TIME;

	UPROPERTY(BlueprintReadOnly)
		bool isFacingRight;
	UPROPERTY(BlueprintReadOnly)
		bool isFacingUp;
	UPROPERTY(BlueprintReadWrite)
		AActor* CollidingActor;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void ChangeHorizontalMovementState(EHorizontalMovementState NewState);

	virtual void ChangeVerticalMovementState(EVerticalMovementState NewState);

	virtual void ChangeMovementState(EMovementState NewState);

	



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void AddMovementInput(FVector2D Direction, float amount);

	virtual void ComputeNewVelocity();

	virtual void ComputeWallMovement(FHitResult HitInfo);

	virtual void ComputeFlyingVelocity();

	virtual void ComputeDashingVelocity();
	
	virtual void ComputeEjectedVelocity();

	virtual void ComputePushVelocity(const AActor* OtherActor);

	virtual void Eject(FVector2D _EjectionSpeed);
		
	virtual void Dash();

	virtual float GetVelocityNorm();
};
