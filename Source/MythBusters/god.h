// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AHitBoxGroup;
#include "CoreMinimal.h"
#include "GodMovementComponent.h"
#include "GodShieldComponent.h"
#include "GodAttackComponent.h"
#include "GodBoostComponent.h"
#include "GameFramework/Actor.h"
#include "god.generated.h"


/*
	The state of a action input
*/
enum EInputActionState
{
	Released,
	Pressed
};

/*
	The type of Input Action or Axis
*/
enum EInputSpecifier
{
	NORMAL,
	SPECIAL,
	PUSH,
	SHIELD,
	DASH,
	HORIZONTAL,
	VERTICAL
};

/*
	Describe an Axis Input (like a joystick for exemple)
*/
struct SInputAxis
{
	float Value;
};


/*
	Describe an Action Input (like a button press for exemple)
*/
struct SInputAction
{
	SInputAction() : State(EInputActionState::Released), PreviousState(EInputActionState::Released) {};
	EInputActionState State;
	EInputActionState PreviousState;
	bool Consumed = false;
};

/*
	Optimize Input struct to minimize packet size when sending online
*/
struct SSendableInputs
{
	float VerticalAxis;
	float HorizontalAxis;
	char Actions;
};

/*
	Describe all the possible Inputs for a God
*/
struct SInputs
{

	SInputAxis VerticalAxis;
	SInputAxis HorizontalAxis;

	SSendableInputs SendableInputs;
	SInputAction InputActions[5];
	void Update(EInputSpecifier Specifier, SInputAxis NewInputAxis)
	{
		switch (Specifier)
		{
			case HORIZONTAL:
				HorizontalAxis = NewInputAxis;
				break;
			case VERTICAL:
				VerticalAxis = NewInputAxis;
				break;
		}
	};
	void Update(EInputSpecifier Specifier, float Value)
	{
		SInputAxis NewInputAxis;
		NewInputAxis.Value = Value;
		Update(Specifier, NewInputAxis);
	};
	void Update(EInputSpecifier Specifier, SInputAction NewInputAction)
	{
		NewInputAction.Consumed = InputActions[Specifier].Consumed;
		InputActions[Specifier] = NewInputAction;
	};
	void Update(EInputSpecifier Specifier, EInputActionState State)
	{
		SInputAction NewInputAction;
		NewInputAction.State = State;
		NewInputAction.PreviousState = InputActions[Specifier].State;
		Update(Specifier, NewInputAction);
	};
	void MakeSendable()
	{
		SendableInputs.HorizontalAxis = HorizontalAxis.Value;
		SendableInputs.VerticalAxis = VerticalAxis.Value;
		char act = 0;
		if (InputActions[NORMAL].State == Pressed){
			act += 0b00000001;
		}
		if (InputActions[SPECIAL].State == Pressed){
			act += 0b00000010;
		}
		if (InputActions[PUSH].State == Pressed){
			act += 0b00000100;
		}
		if (InputActions[DASH].State == Pressed){
			act += 0b00001000;
		}
		if (InputActions[SHIELD].State == Pressed){
			act += 0b00010000;
		}
		SendableInputs.Actions = act;
	};
	void Readable(SSendableInputs const * const Inputs)
	{
		if (Inputs != nullptr)
		{
			HorizontalAxis.Value = Inputs->HorizontalAxis;
			VerticalAxis.Value = Inputs->VerticalAxis;
			InputActions[NORMAL].State = ((Inputs->Actions & 0b00000001) != 0) ? Pressed : Released;
			InputActions[SPECIAL].State = ((Inputs->Actions & 0b00000010) != 0) ? Pressed : Released;
			InputActions[PUSH].State = ((Inputs->Actions & 0b00000100) != 0) ? Pressed : Released;
			InputActions[DASH].State = ((Inputs->Actions & 0b00001000) != 0) ? Pressed : Released;
			InputActions[SHIELD].State = ((Inputs->Actions & 0b00010000) != 0) ? Pressed : Released;
		}		
	};
};

/*
	All the state a God can be in
*/
UENUM(BlueprintType)
enum class EGodState : uint8 {
	Flying UMETA(DisplayName = "Flying"),
	FlyingTurnaround UMETA(DisplayName = "FlyingTurnaround"),
	Dashing UMETA(DisplayName = "Dashing"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Ejected UMETA(DisplayName = "Ejected"),
	WallHit,
	Hurt UMETA(DisplayName = "Hurt"),
	Attacking UMETA(DisplayName = "Attacking"),
	Shielding UMETA(DisplayName = "Shielding"),
	Dead UMETA(DisplayName = "Dead")
};


/*
	Character class for all the Playable Characters
*/
UCLASS()
class MYTHBUSTERS_API AGod : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGod();
	
	// Hurtbox and collision handling
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;
	// Total damage received by the god
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		float GodDamage = 0.0f;

	// Input Storage
	SInputs Inputs;
	// Online Input Storage
	SInputs GGPOInputs;
	// Read Input from variable to execute specific actions
	void ReadInputs(SInputs* Inputs);
	// Write Axis Input Value in a variable
	void WriteInputs(EInputSpecifier Specifier, float Value);
	// Write Action Input State in a variable
	void WriteInputs(EInputSpecifier Specifier, EInputActionState State);
	void WriteHorizontalAxis(float Value);
	void WriteVerticalAxis(float Value);
	void PressAttackNormal();
	void ReleaseAttackNormal();
	void PressAttackSpecial();
	void ReleaseAttackSpecial();
	void PressAttackPush();
	void ReleaseAttackPush();
	void PressShield();
	void ReleaseShield();
	void PressDash();
	void ReleaseDash();

protected:
	// Appearance of the character
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SkeletalMesh;
	// Component to handle all flying mouvement (dashing, turnaround...)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Movement", meta = (AllowPrivateAccess = "true"))
		UGodMovementComponent* GodMovement;
	// Component to handle shield Ability
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		UGodShieldComponent* GodShield;
	// The state of attack the character is in
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		EAttackDirection attackState;
	// Component to describe all the attacks a character can do
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		UGodAttackComponent* GodAttack;
	// Component to handle all the boost a character has
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Boost", meta = (AllowPrivateAccess = "true"))
		UGodBoostComponent* GodBoost;

	// Joystick deadzones
	float HorizontalDeadZone = 0.15f;
	float VerticalDeadZone = 0.15f;
	
	//If the character is online
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool netplay = false;

	// Boolean to limit character ability in certain game modes
	bool canDash = true;
	bool canAttNorm = true;
	bool canAttSpe = true;
	bool canAttPush = true;
	bool canShield = true;

	// Time window (in frame) to input a backward attack during the beginning of a turnaround
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
	int BackwardAttackFrameWindow = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	    virtual void MoveHorizontal(float AxisValue);
	UFUNCTION(BlueprintImplementableEvent)
		void EMoveHorizontal(float AxisValue);

	UFUNCTION(BlueprintCallable)
	    virtual void MoveVertical(float AxisValue);
	UFUNCTION(BlueprintImplementableEvent)
	    void EMoveVertical(float AxisValue);


	UFUNCTION(BlueprintCallable)
		virtual void AttackNormal();
	UFUNCTION(BlueprintImplementableEvent)
	    void EAttackNormal();

	UFUNCTION(BlueprintCallable)
		virtual void StopAttackNormal();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopAttackNormal();


	UFUNCTION(BlueprintCallable)
		virtual void AttackSpecial();
	UFUNCTION(BlueprintImplementableEvent)
		void EAttackSpecial();

	UFUNCTION(BlueprintCallable)
		virtual void StopAttackSpecial();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopAttackSpecial();


	UFUNCTION(BlueprintCallable)
		virtual void AttackPush();
	UFUNCTION(BlueprintImplementableEvent)
		void EAttackPush();

	UFUNCTION(BlueprintCallable)
		virtual void StopAttackPush();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopAttackPush();

	   
	UFUNCTION(BlueprintCallable)
		virtual void Shield();
	UFUNCTION(BlueprintImplementableEvent)
		void EShield();

	UFUNCTION(BlueprintCallable)
		virtual void StopShield();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopShield();


	UFUNCTION(BlueprintCallable)
		virtual void Eject(FVector2D _EjectionSpeed);

	UFUNCTION(BlueprintCallable)
	virtual void Dash();
	UFUNCTION(BlueprintCallable)
		virtual void StopDash();

	void Die();

	UFUNCTION(BlueprintCallable)
		virtual void SetGodDamage(float value) { this->GodDamage = value; }
	UFUNCTION(BlueprintCallable)
		virtual float GetGodDamage() { return this->GodDamage; }
	UFUNCTION(BlueprintCallable)
		virtual void ApplyGodDamage(float value);

	UFUNCTION(BlueprintImplementableEvent)
		void TurnaroundEvent();

	void UpdateState(EMovementState NewMovementState);
	void UpdateAttackState();

	void InstantTurn();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    /// Called to bind functionality to input
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
	/// Called to Setup what the god can do
	UFUNCTION(BlueprintCallable)
	void SetupGodLimitation(bool _canDash, bool _canAttNorm, bool _canAttSpe, bool _canAttPush, bool _canShield);

	/// Called by GodAnimInstance to get values nec. for animations blendSpace
	virtual float GetAnimValues(int _idValueToGet);
	/// Called to set up the skeletal mesh custom depth pencil
	UFUNCTION(BlueprintCallable)
	void SetPlayerColor(int depth);
	
	/// Allow to change good state properly (with canMove and other ajustements)
	//UFUNCTION(BlueprintCallable)
	virtual void ChangeGodState(EGodState NewState);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EGodState State;

	bool canMove = true;

	int TurnTimer = 0;

	// Component getters methods
	USkeletalMeshComponent* GetSkeletalMesh();
	UGodMovementComponent* GetGodMovementComponent() { return GodMovement; };
	UGodAttackComponent* GetGodAttackComponent() { return GodAttack; };
	UGodBoostComponent* GetGodBoostComponent() { return GodBoost; };
	UGodShieldComponent* GetGodShieldComponent() { return GodShield; };

	/// Method called when the god hits a hitBoxGroup
	UFUNCTION(BlueprintCallable)
	void HandleHitBoxGroupCollision(AHitBoxGroup* hitBoxGroup);

	/// Method to call to transmit attack anim notify to GodAttackComponent
	void HandleAttackNotify(ENotifyType notifyType);
	/// Method to call to transmit Porjectile to register to the GodAttackComponent
	void RegisterProjectile(AHitBoxGroupProjectile* _projectile, int _idAttack);
	// Method to add GodSelection to action char
	//static char SendGodSelection(char act);
	//Method to read GodSelection and modify gamestate
	//static void ReadGodSelection(char act);
};

/*
	Class to store the whole state of the character each frame.
	It is only the necessary variables to completely describe the character.
	Useful for rollbacks.

*/
struct SAbstractGod
{
	AGod* Ref;
	FTransform Transform;

	float GodDamage;

	bool canMove;

	int TurnTimer;

	EGodState State;



	FAttacksSaveState AttackSaveState;
	SMovementSaveState MovementSaveState;
	TArray<FBoost> Boosts;
	FShieldSaveState ShieldSaveState;


	void Init(AGod* ref)
	{
		Ref = ref;
		AttackSaveState = FAttacksSaveState();
		Boosts.Init(FBoost(), 5);
		MovementSaveState = SMovementSaveState();
		MovementSaveState.Init(Ref->GetGodMovementComponent());
		ShieldSaveState = FShieldSaveState();
	};

	void Observe()
	{
		Transform = Ref->GetActorTransform();
		GodDamage = Ref->GodDamage;
		canMove = Ref->canMove;
		TurnTimer = Ref->TurnTimer;
		State = Ref->State;
		MovementSaveState.Observe();
		AttackSaveState = Ref->GetGodAttackComponent()->SaveAttacksState();
		Boosts = Ref->GetGodBoostComponent()->SaveBoostState();
		ShieldSaveState = Ref->GetGodShieldComponent()->SaveShieldState();
	};

	void Apply()
	{
		Ref->SetActorTransform(Transform);
		Ref->GodDamage = GodDamage;
		Ref->canMove = canMove;
		Ref->TurnTimer = TurnTimer;
		Ref->State = State;
		MovementSaveState.Apply();
		Ref->GetGodAttackComponent()->LoadAttacksState(AttackSaveState);
		Ref->GetGodBoostComponent()->LoadBoostState(Boosts);
		Ref->GetGodShieldComponent()->LoadShieldState(ShieldSaveState);
	};
};
