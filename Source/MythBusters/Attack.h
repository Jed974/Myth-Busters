// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class AGod;
class UGodAnimInstance;

#include "CoreMinimal.h"
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
	AIM
};

struct UAttackSaveState {
	EAttackState attackState_Saved;
	float animationFrame_Saved;
	// Other, car be derived for attacks that need more information as projectile position

	UAttackSaveState() : attackState_Saved(EAttackState::OFF), animationFrame_Saved(-1) {};
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
	UPROPERTY(Category = "State", VisibleAnywhere, BlueprintReadWrite)
	EAttackState attackState;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite)
	UAnimMontage* attackMontage;
	//UPROPERTY(Category = "God Pointers", VisibleAnywhere)
	AGod* god;
	//UPROPERTY(Category = "God Pointers", EditAnywhere)
	UGodAnimInstance* godAnimInstance;

	virtual void PlayMontageFromCurrentState(float _animationFrameToLoad);
	UFUNCTION(BlueprintCallable)
	/** Method called to call when the attack is over (triggered by OverNotify) */
	virtual void OverAttack();


public:	
	UAttack();
	virtual ~UAttack();
	
	/** Method called to start the attack*/
	virtual void StartAttack();
	/** Method called to stop the attack at any moment*/
	virtual void StopAttack();

	virtual void ApplySaveState(UAttackSaveState _saveState);
	virtual UAttackSaveState GetSaveState();
	virtual void LoadAttackSaveState(EAttackState _stateToLoad, float _animationFrameToLoad = -1);
	virtual float GetAttackFrame();
	virtual EAttackState GetAttackState();

	virtual void OnOverNotify();		// Déclenchée par une anim notify indiquant la fin de l'attaque
	virtual void OnActiveNotify();		// Déclenchée par une anim notify indiquant l'apparition de la hitbox ou de l'effet
	virtual void OnChangeNotify();		// Déclenchée par une anim notify indiquant un changement de hitbox ou de l'effet
	virtual void OnInactiveNotify();	// Déclenchée par une anim notify indiquant la disparition de la hitbox ou de l'effet

	void SetUpAttack(AGod* _god, int _idAttack);
	const EAttackState getState();
};