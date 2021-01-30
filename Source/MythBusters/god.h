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



enum EInputActionState
{
	Released,
	Pressed
};

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

struct SInputAxis
{
	float Value;
};

struct SInputAction
{
	SInputAction() : State(EInputActionState::Released), PreviousState(EInputActionState::Released) {};
	EInputActionState State;
	EInputActionState PreviousState;
	bool Consumed = false;
};

struct SSendableInputs
{
	float VerticalAxis;
	float HorizontalAxis;
	char Actions;
};

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
		//act = AGod::SendGodSelection(act);
		act = SendGodSelection(act);
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
			//AGod::ReadGodSelection(Inputs->Actions);
			ReadGodSelection(Inputs->Actions);
		}		
	};
	char SendGodSelection(char act);
	void ReadGodSelection(char act);
};

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

UCLASS()
class MYTHBUSTERS_API AGod : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGod();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		float GodDamage = 0.0f;

	SInputs Inputs;
	SInputs GGPOInputs;
	void ReadInputs(SInputs* Inputs);
	void WriteInputs(EInputSpecifier Specifier, float Value);
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
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Movement", meta = (AllowPrivateAccess = "true"))
		UGodMovementComponent* GodMovement;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		UGodShieldComponent* GodShield;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		EAttackDirection attackState;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		UGodAttackComponent* GodAttack;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Boost", meta = (AllowPrivateAccess = "true"))
		UGodBoostComponent* GodBoost;


	float HorizontalDeadZone = 0.15f;
	float VerticalDeadZone = 0.15f;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool netplay = false;

	// Boolean de GameMode (ne sont pas sensé changer au cours d'une partie) :
	bool canDash = true;
	bool canAttNorm = true;
	bool canAttSpe = true;
	bool canAttPush = true;
	bool canShield = true;

	
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
