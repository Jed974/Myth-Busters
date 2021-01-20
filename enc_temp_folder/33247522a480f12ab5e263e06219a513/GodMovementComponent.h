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

UENUM(BlueprintType)
enum class EHorizontalMovementState : uint8
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

UENUM(BlueprintType)
enum class EVerticalMovementState : uint8
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

	/** The time (in frames) it takes before being able to cancel the fly stop movement into an other horizontal movement */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int HorizontalFlyStopCancelFrames;

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

	/** The time (in frames) it takes before being able to cancel the fly stop movement into an other vertical movement */
	UPROPERTY(Category = "God Movement: Flying", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		int VerticalFlyStopCancelFrames;

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

	/** Dashing speed */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float DashingSpeed;

	/** Number of frames before starting a dash */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
		int DashStartupFrames;

	/** Number of frames in a dash */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
		int DashFrames;

	/** Number of frames where character can't move after a dash */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
		int DashLagFrames;

	/** The evolution of the speed during a dash */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere)
		FRuntimeFloatCurve DashSpeedProfile;
	

	/** Dashing speed, as a multiplier of maximum flying speed */
	UPROPERTY(Category = "God Movement: Dash & Sprint", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float SprintSpeedScale;

	

	EHorizontalMovementState HorizontalMovementState;
	EVerticalMovementState VerticalMovementState;

	UPROPERTY(BlueprintReadOnly)
	FVector2D Velocity;
	FVector2D DashDir;
	FVector2D PushVelocity;
	UPROPERTY(BlueprintReadOnly)
	bool IsPushable;
	FVector2D EjectionVelocity;

	FVector2D _MovementInput;

	float HorizontalSpeed;
	float HorizontalPreviousSpeed;
	int CurrentHorizontalStateTimer;

	int CurrentVerticalStateTimer;
	float VerticalSpeed;
	float VerticalPreviousSpeed;

	int DashFrameCounter = 0;
	int DashStartupCounter = 0;
	int DashLagCounter = 0;
	int EjectionFrameCounter = 0;
	int WallHitFrameCounter = 0;

	UPROPERTY(BlueprintReadOnly)
		bool isFacingRight;
	UPROPERTY(BlueprintReadOnly)
		bool isFacingUp;
	UPROPERTY(BlueprintReadWrite)
		AActor* CollidingActor;

protected:
	
	
	
	float DELTA_TIME;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void ChangeHorizontalMovementState(EHorizontalMovementState NewState);

	virtual void ChangeVerticalMovementState(EVerticalMovementState NewState);

	virtual void ChangeMovementState(EMovementState NewState);

	



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
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

	bool GetIsFacingRight();
};


struct SMovementSaveState
{
	UGodMovementComponent* Ref;
	EMovementState MovementState;
	EHorizontalMovementState HorizontalMovementState;
	EVerticalMovementState VerticalMovementState;
	FVector2D DashDir;
	bool IsPushable;

	float HorizontalSpeed;
	float HorizontalPreviousSpeed;
	int CurrentHorizontalStateTimer;

	int CurrentVerticalStateTimer;
	float VerticalSpeed;
	float VerticalPreviousSpeed;

	int DashFrameCounter;
	int DashStartupCounter;
	int DashLagCounter;
	int EjectionFrameCounter;
	int WallHitFrameCounter;

	bool isFacingRight;
	bool isFacingUp;

	AActor* CollidingActor;

	void Init(UGodMovementComponent* ref)
	{
		Ref = ref;
	}

	void Observe()
	{
		MovementState = Ref->MovementState;
		HorizontalMovementState = Ref->HorizontalMovementState;
		VerticalMovementState = Ref->VerticalMovementState;
		DashDir = Ref->DashDir;
		IsPushable = Ref->IsPushable;

		HorizontalSpeed = Ref->HorizontalSpeed;
		HorizontalPreviousSpeed = Ref->HorizontalPreviousSpeed;
		CurrentHorizontalStateTimer = Ref->CurrentHorizontalStateTimer;

		CurrentVerticalStateTimer = Ref->CurrentVerticalStateTimer;
		VerticalSpeed = Ref->VerticalSpeed;
		VerticalPreviousSpeed = Ref->VerticalPreviousSpeed;

		DashFrameCounter = Ref->DashFrameCounter;
		DashStartupCounter = Ref->DashStartupCounter;
		DashLagCounter = Ref->DashLagCounter;
		EjectionFrameCounter = Ref->EjectionFrameCounter;
		WallHitFrameCounter = Ref->WallHitFrameCounter;

		isFacingRight = Ref->isFacingRight;
		isFacingUp = Ref->isFacingUp;

		CollidingActor = Ref->CollidingActor;
	};

	void Apply()
	{
		Ref->MovementState = MovementState;
		Ref->HorizontalMovementState = HorizontalMovementState;
		Ref->VerticalMovementState = VerticalMovementState;
		Ref->DashDir = DashDir;
		Ref->IsPushable = IsPushable;

		Ref->HorizontalSpeed = HorizontalSpeed;
		Ref->HorizontalPreviousSpeed = HorizontalPreviousSpeed;
		Ref->CurrentHorizontalStateTimer = CurrentHorizontalStateTimer;

		Ref->CurrentVerticalStateTimer = CurrentVerticalStateTimer;
		Ref->VerticalSpeed = VerticalSpeed;
		Ref->VerticalPreviousSpeed = VerticalPreviousSpeed;

		Ref->DashFrameCounter = DashFrameCounter;
		Ref->DashStartupCounter = DashStartupCounter;
		Ref->DashLagCounter = DashLagCounter;
		Ref->EjectionFrameCounter = EjectionFrameCounter;
		Ref->WallHitFrameCounter = WallHitFrameCounter;

		Ref->isFacingRight = isFacingRight;
		Ref->isFacingUp = isFacingUp;

		Ref->CollidingActor = CollidingActor;
	}

};