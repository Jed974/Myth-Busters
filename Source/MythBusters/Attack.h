// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class AGod;
class UGodAnimInstance;

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "UObject/NoExportTypes.h"
#include "Attack.generated.h"


/// Enum used to keep track of the attack's state in order to perform rollbacks with ggpo
UENUM(BlueprintType)
enum class EAttackState : uint8
{
	OFF,
	START,
	RECOVER,
	HITACTIVE1,
	HITACTIVE2,
	HITACTIVE3,
	AIM,
	COOLDOWN
};

/// Attack's SaveState
struct UAttackSaveState {
	/// The attack's state
	EAttackState attackState_Saved;
	/// The attack's montage current frame
	float animationFrame_Saved;
	/// The attack's current position on the inducedMovement curve
	int inducedMovementAbscissa;
	/// Auxiliary information, in case of a singlehit or multihit attack indicates if the hitBoxGroup has already hit the other god
	bool auxAlreadyHit;
	/// AUxiliary information, for example in sequential attack gives the current subattack
	int auxiliaryInfo;

	UAttackSaveState() : attackState_Saved(EAttackState::OFF), animationFrame_Saved(-1), auxAlreadyHit(false), auxiliaryInfo(-1) {};
};

/**
 * Base class for every attack. Should not be blueprint derivated
 * Attacks have a montage associated. This montage must always ends with an anim Notify "AttackAnimOver"
 */
UCLASS(Blueprintable)
class MYTHBUSTERS_API UAttack : public UObject
{
	GENERATED_BODY()

protected:
	/// Id of the attack in the GodAttackComponent's Attacks array
	int idAttackOnGodAttackComponent = -1;
	/// Auxiliary information, for example if the attack is a subAttack from a sequencialAttack, it will be the subAttack id in the sequencialAttack's Attacks array
	int auxiliaryInfo = -1;
	/// Current state of the attack (doesn't drive logic, only witnesses it + used to rollback)
	UPROPERTY(Category = "State", VisibleAnywhere, BlueprintReadWrite)
	EAttackState attackState;
	/// Number of frame cooldown to wait before being able to re-performs the attack after it is over ; -1 is no cooldown
	UPROPERTY(Category = "State", EditAnywhere, BlueprintReadOnly)
	int cooldDownAmount = -1;
	/// Current cooldown timer
	int cooldDownFrame = 0;
	/// AnimMontage to play when performing the attack ; blendIn, blendOut must be at 0
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite)
	UAnimMontage* attackMontage;
		
	/// Pointer to the god owning the attack
	AGod* god;
	/// Pointer to the animIntacne of the god owning the attack
	UGodAnimInstance* godAnimInstance;

	/// Vector Curve giving the movement induced by an attack over time. Only use X and Y. If no induced movement is wanted set the value to the "AtkCurve_NoMovement" uasset.
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadOnly)
	UCurveVector* InducedMovement;
	/// Current abscissa (or time) on the InducedMovement curve
	int curveAbscissa = 0;

	/// Maximum movement speed allowed during the attack (independant of the induced movement)
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadOnly)
	float MaxDirectionalInfluenceSpeed;
	/// Acceleration with which the MaxDirectionalInfluenceSpeed can be reached
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadOnly)
	float DirectionalInfluenceAcceleration;

	/// Start to play attack's montage from the time given. Can be override to play a different montage depending of the attack's state
	virtual void PlayMontageFromCurrentState(float _animationFrameToLoad);


public:	
	UAttack();
	virtual ~UAttack();
	
	/** Method called to start the attack*/
	virtual void StartAttack();
	/** Method called to stop the attack at any moment*/
	virtual void StopAttack();
	/// Method called to call when the attack is over (triggered by OverNotify)
	virtual void OverAttack();
	/** Method called to set up the attack for further use*/
	virtual void SetUpAttack(AGod* _god, int _idAttack, int _auxInfo = -1);

	/// Apply the given Attack SaveState given to the attack with the option to not play/resume the animation (useful for sequential attacks)
	virtual void ApplySaveState(UAttackSaveState _saveState, bool _playAnimation = true);
	/// Returns an Attack SaveState based on the current attack's properties
	virtual UAttackSaveState GetSaveState();
	/// Get the time at which the attack's montage is
	virtual float GetAttackFrame();
	/// Get the attack's sate, overridable
	virtual EAttackState GetAttackState();
	/// Get the attack's sate, not overridable (GetAttackState was sometimes not working)
	EAttackState GAS();

	/// Triggered by an animNotify "AttackAnimOver" and indicates the end of the attack
	virtual void OnOverNotify();
	/// Triggered by an animNotify "AttackAnimActive" and triggers attack's effect
	virtual void OnActiveNotify();
	/// Triggered by an animNotify "AttackAnimChange" and changes attack's effect
	virtual void OnChangeNotify();
	/// Triggered by an animNotify "AttackAnimInactive" and ends attack's effect
	virtual void OnInactiveNotify();
	/// Triggered by an animNotify "AttackAnimNext" and indicates the end of a subAttack or a subMontage
	virtual void OnNextNotify();

	/// Decreases cooldown and change attack state if the cooldown is over
	void DecreaseCoolDown();
	/// Returns current cooldown frame
	int GetCoolDown() const;
	/// Sets cooldown frame
	void SetCoolDown(int _cd);

	/// Returns the speed vector from the inducedMovement curve based on the curveAbscissa + increments curveAbscissa
	FVector2D GetInducedMovement();
	/// Returns MaxDirectionalInfluenceSpeed
	float GetMaxDirectionalInfluenceSpeed() { return MaxDirectionalInfluenceSpeed; };
	/// Returns DirectionalInfluenceAcceleration
	float GetDirectionalInfluenceAcceleration() { return DirectionalInfluenceAcceleration; };

	/// Returns the attack's auxiliary Info
	int GetAuxInfo();
};