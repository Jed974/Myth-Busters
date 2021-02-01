// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UAttackProjectile;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackSequencial.generated.h"

/**
 * Special kind of attack that play several subAttacks on a unique montage in once
 * Anim notifies to use :
 * - ... (notifies needed by the first subAttack except AttackAnimOver)
 * - AttackAnimNext (as many as ther are subAttack to trigger
 * - ... (idem for the current subAttack)
 * 
 * Of course, ends the attack with AttackAnimOver
 */
UCLASS()
class MYTHBUSTERS_API UAttackSequencial : public UAttack
{
	GENERATED_BODY()

protected :
	/// Classes of the different attacks to perform, first attack being the one with index 0
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attacks", meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<UAttack>> AttacksToPerform;

	/// Instances of the attacks to perform
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Attacks", meta = (AllowPrivateAccess = "true"))
	TArray<UAttack*> Attacks;

	/// Current subAttack index in Attacks.
	int idCurrentAttack = 0;


public :
	UAttackSequencial();
	//virtual ~UAttackSequencial();

	virtual void StartAttack() override;
	virtual void StopAttack() override;
	virtual void OverAttack() override;
	virtual void SetUpAttack(AGod* _god, int _idAttack, int _auxInfo = -1) override;

	virtual void ApplySaveState(UAttackSaveState _saveState, bool _playAnimation = true) override;
	virtual UAttackSaveState GetSaveState() override;
	virtual EAttackState GetAttackState() override;

	virtual void OnActiveNotify() override;
	virtual void OnInactiveNotify() override;
	virtual void OnChangeNotify() override;
	virtual void OnNextNotify() override;

	/// Tries to cast Attacks[id_Att] into a UAttackProjectile, put the result into outAttack and returns if it was successful
	bool GetProjectileAttack(int id_Att, UAttackProjectile* &outAttack);
};
