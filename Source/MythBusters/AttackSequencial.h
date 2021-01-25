// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UAttackProjectile;
#include "CoreMinimal.h"
#include "Attack.h"
#include "AttackSequencial.generated.h"

/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UAttackSequencial : public UAttack
{
	GENERATED_BODY()

protected :
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attacks", meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<UAttack>> AttacksToPerform;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Attacks", meta = (AllowPrivateAccess = "true"))
	TArray<UAttack*> Attacks;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Attacks", meta = (AllowPrivateAccess = "true"))
	int idCurrentSubAttack = 0;


public :
	UAttackSequencial();
	//virtual ~UAttackSequencial();

	virtual void StartAttack() override;
	virtual void StopAttack() override;
	virtual void OverAttack() override;
	virtual void SetUpAttack(AGod* _god, int _idAttack, int _auxInfo = -1) override;

	virtual void ApplySaveState(UAttackSaveState _saveState) override;
	virtual UAttackSaveState GetSaveState() override;
	virtual void LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad = -1, bool LoadAnimation = true) override;
	virtual EAttackState GetAttackState() override;

	virtual void OnActiveNotify() override;
	virtual void OnInactiveNotify() override;
	virtual void OnChangeNotify() override;
	virtual void OnNextNotify() override;

	bool GetProjectileAttack(int id_Att, UAttackProjectile* &outAttack);
};
