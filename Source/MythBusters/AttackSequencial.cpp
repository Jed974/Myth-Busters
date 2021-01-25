// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSequencial.h"
#include "AttackProjectile.h"



UAttackSequencial::UAttackSequencial() : UAttack() {
	Attacks.Empty();
}
//UAttackSequencial::~UAttackSequencial() : ~UAttack() {}

void UAttackSequencial::StartAttack() {
	Super::StartAttack();
	idCurrentSubAttack = 0;
}
void UAttackSequencial::StopAttack() {
	for (int i = 0; i < Attacks.Num(); i++) {
		if (Attacks[i] != nullptr)
			Attacks[i]->StopAttack();
	}
}
void UAttackSequencial::OverAttack() {
	Super::OverAttack();
	idCurrentSubAttack = -1;
	for (int i = 0; i < Attacks.Num(); i++) {
		if (Attacks[i] != nullptr)
			Attacks[i]->OverAttack();
	}
}
void UAttackSequencial::SetUpAttack(AGod* _god, int _idAttack, int _auxInfo) {
	Super::SetUpAttack(_god, _idAttack, _auxInfo);

	Attacks.Init(nullptr, AttacksToPerform.Num());
	for (int i = 0; i < AttacksToPerform.Num(); i++) {
		Attacks[i] = NewObject<UAttack>(this, *AttacksToPerform[i]);
		Attacks[i]->SetUpAttack(_god, _idAttack, i);
	}
}


void UAttackSequencial::ApplySaveState(UAttackSaveState _saveState) {
	idCurrentSubAttack = 3;
	int ntm = _saveState.auxiliaryInfo;
	idCurrentSubAttack = ntm;
	Super::ApplySaveState(_saveState);
}
UAttackSaveState UAttackSequencial::GetSaveState() {
	UAttackSaveState svst = Super::GetSaveState();
	svst.auxiliaryInfo = idCurrentSubAttack;
	return svst;
}
void UAttackSequencial::LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad, bool LoadAnimation) {
	if (_stateToLoad == EAttackState::OFF)
		OverAttack();
	else {
		attackState = EAttackState::START;

		// if currentAttack is aim attack + state of wait -> aim attack handles animation
			// stuff....
		//else
		if (Attacks[idCurrentSubAttack] != nullptr)
			Attacks[idCurrentSubAttack]->LoadAtAttackStateAndFrame(_stateToLoad, -1, false);

		PlayMontageFromCurrentState(_animationFrameToLoad);	// the animation is handle by sequential animation only
	}
}
EAttackState UAttackSequencial::GetAttackState() {
	if (attackState == EAttackState::OFF)
		return EAttackState::OFF;		// If the attack is off, we tell so
	else {
		if (Attacks[idCurrentSubAttack] != nullptr) {
			EAttackState subState = Attacks[idCurrentSubAttack]->GAS();//GetAttackState();
			if (subState != EAttackState::OFF)
				return subState;
			else
				return EAttackState::START;		// We want to keep OFF only when the attack is completely OFF, otherwise it's because we had the current subattack on its Start phase
		}
		else
			return EAttackState::OFF;
	}
}


void UAttackSequencial::OnActiveNotify() {
	if (idCurrentSubAttack != -1 && idCurrentSubAttack < Attacks.Num() && Attacks[idCurrentSubAttack] != nullptr)
		Attacks[idCurrentSubAttack]->OnActiveNotify();
}
void UAttackSequencial::OnInactiveNotify() {
	if (idCurrentSubAttack != -1 && idCurrentSubAttack < Attacks.Num() && Attacks[idCurrentSubAttack] != nullptr)
		Attacks[idCurrentSubAttack]->OnInactiveNotify();
}
void UAttackSequencial::OnChangeNotify() {
	if (idCurrentSubAttack != -1 && idCurrentSubAttack < Attacks.Num() && Attacks[idCurrentSubAttack] != nullptr)
		Attacks[idCurrentSubAttack]->OnChangeNotify();
}
void UAttackSequencial::OnNextNotify() {
	if (idCurrentSubAttack != -1 && idCurrentSubAttack < Attacks.Num() && Attacks[idCurrentSubAttack] != nullptr)
		Attacks[idCurrentSubAttack]->OverAttack();

	idCurrentSubAttack += 1;

	//Security
	if (idCurrentSubAttack >= Attacks.Num()) {
		OverAttack();
	}
}

bool UAttackSequencial::GetProjectileAttack(int id_Att, UAttackProjectile* &outAttack) {
	if (idCurrentSubAttack != -1 && idCurrentSubAttack < Attacks.Num() && Attacks[idCurrentSubAttack] != nullptr) {
		outAttack = Cast<UAttackProjectile>(Attacks[id_Att]);
		return (outAttack != nullptr);
	}
	else {
		outAttack = nullptr;
		return false;
	}
}