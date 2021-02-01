// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSequencial.h"
#include "AttackProjectile.h"



UAttackSequencial::UAttackSequencial() : UAttack() {
	Attacks.Empty();
}
//UAttackSequencial::~UAttackSequencial() : ~UAttack() {}

void UAttackSequencial::StartAttack() {
	Super::StartAttack();
	idCurrentAttack = 0;
}
void UAttackSequencial::StopAttack() {
	for (int i = 0; i < Attacks.Num(); i++) {
		if (Attacks[i] != nullptr)
			Attacks[i]->StopAttack();
	}
}
void UAttackSequencial::OverAttack() {
	Super::OverAttack();
	idCurrentAttack = -1;
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


void UAttackSequencial::ApplySaveState(UAttackSaveState _saveState, bool _playAnimation) {
	idCurrentAttack = _saveState.auxiliaryInfo;
	//if (_saveState.attackState_Saved != EAttackState::OFF)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat((int)_saveState.attackState_Saved));
	//curveAbscissa = _saveState.inducedMovementAbscissa;
	Super::ApplySaveState(_saveState, _playAnimation);
		/*curveAbscissa = _saveState.inducedMovementAbscissa;
		attackState = _saveState.attackState_Saved;
		if (_playAnimation)
			PlayMontageFromCurrentState(_saveState.animationFrame_Saved);*/

	if (_saveState.attackState_Saved == EAttackState::OFF)
		OverAttack();
	else {
		attackState = EAttackState::START;

		/* if currentAttack is aim attack + state of wait -> aim attack handles animation
			 stuff....
		else*/

		if (Attacks[idCurrentAttack] != nullptr)
			Attacks[idCurrentAttack]->ApplySaveState(_saveState, false); //LoadAtAttackStateAndFrame(_stateToLoad, -1, false);

		PlayMontageFromCurrentState(_saveState.animationFrame_Saved);	// the animation is handle by sequential animation only
	}
}
UAttackSaveState UAttackSequencial::GetSaveState() {
	UAttackSaveState svst = Super::GetSaveState();
	svst.auxiliaryInfo = idCurrentAttack;
	return svst;
}
EAttackState UAttackSequencial::GetAttackState() {
	if (attackState == EAttackState::OFF)
		return EAttackState::OFF;		// If the attack is off, we tell so
	else {
		if (Attacks[idCurrentAttack] != nullptr) {
			EAttackState subState = Attacks[idCurrentAttack]->GAS();//GetAttackState();
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
	if (idCurrentAttack != -1 && idCurrentAttack < Attacks.Num() && Attacks[idCurrentAttack] != nullptr)
		Attacks[idCurrentAttack]->OnActiveNotify();
}
void UAttackSequencial::OnInactiveNotify() {
	if (idCurrentAttack != -1 && idCurrentAttack < Attacks.Num() && Attacks[idCurrentAttack] != nullptr)
		Attacks[idCurrentAttack]->OnInactiveNotify();
}
void UAttackSequencial::OnChangeNotify() {
	if (idCurrentAttack != -1 && idCurrentAttack < Attacks.Num() && Attacks[idCurrentAttack] != nullptr)
		Attacks[idCurrentAttack]->OnChangeNotify();
}
void UAttackSequencial::OnNextNotify() {
	if (idCurrentAttack != -1 && idCurrentAttack < Attacks.Num() && Attacks[idCurrentAttack] != nullptr)
		Attacks[idCurrentAttack]->OverAttack();

	idCurrentAttack += 1;

	//Security
	if (idCurrentAttack >= Attacks.Num()) {
		OverAttack();
	}
}

bool UAttackSequencial::GetProjectileAttack(int id_Att, UAttackProjectile* &outAttack) {
	outAttack = Cast<UAttackProjectile>(Attacks[id_Att]);
	return (outAttack != nullptr);
}