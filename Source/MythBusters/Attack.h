// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class AGod;
class UGodAnimInstance;

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "UObject/NoExportTypes.h"
#include "Attack.generated.h"


UENUM(BlueprintType)
enum class EAttackState : uint8 	// A completer selon les types d'attaques
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

struct UAttackSaveState {
	EAttackState attackState_Saved;
	float animationFrame_Saved;
	int inducedMovementAbscissa;
	int auxiliaryInfo;
	// Other, car be derived for attacks that need more information as projectile position

	UAttackSaveState() : attackState_Saved(EAttackState::OFF), animationFrame_Saved(-1), auxiliaryInfo(-1){};
};

/**
 * 
 */
UCLASS(Blueprintable)
class MYTHBUSTERS_API UAttack : public UObject
{
	GENERATED_BODY()

protected:
	int idAttackOnGodAttackComponent = -1;
	int auxiliaryInfo = -1;
	UPROPERTY(Category = "State", VisibleAnywhere, BlueprintReadWrite)
	EAttackState attackState;
	UPROPERTY(Category = "State", EditAnywhere, BlueprintReadOnly)
	int cooldDownAmount = -1;
	int cooldDownFrame = 0;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite)
	UAnimMontage* attackMontage;

	AGod* god;
	UGodAnimInstance* godAnimInstance;


	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadOnly)
	UCurveVector* InducedMovement;
	int curveAbscissa = 0;

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

	virtual void ApplySaveState(UAttackSaveState _saveState);
	virtual UAttackSaveState GetSaveState();
	virtual void LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad = -1, bool LoadAnimation = true);
	virtual float GetAttackFrame();
	virtual EAttackState GetAttackState();
	EAttackState GAS();

	virtual void OnOverNotify();		// Déclenchée par une anim notify indiquant la fin de l'attaque
	virtual void OnActiveNotify();		// Déclenchée par une anim notify indiquant l'apparition de la hitbox ou de l'effet
	virtual void OnChangeNotify();		// Déclenchée par une anim notify indiquant un changement de hitbox ou de l'effet
	virtual void OnInactiveNotify();	// Déclenchée par une anim notify indiquant la disparition de la hitbox ou de l'effet
	virtual void OnNextNotify();		// Déclenchée par une anim notify indiquant la fin d'un montage ou attaque intermédiaire

	void DecreaseCoolDown();
	int GetCoolDown() const;
	void SetCoolDown(int _cd);

	FVector2D GetInducedMovement();
};