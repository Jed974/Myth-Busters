#include "Attack.h"
#include "god.h"
#include "GodAnimInstance.h"


UAttack::UAttack() {
	attackState = EAttackState::OFF;
}

UAttack::~UAttack() {}


void UAttack::PlayMontageFromCurrentState(float _animationFrameToLoad) {
	if (attackState != EAttackState::OFF) {
		if (_animationFrameToLoad < 0)
			godAnimInstance->Montage_Play(attackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0, true); // 0 -> _animationFrameToLoad / 60
		else
			godAnimInstance->Montage_Play(attackMontage, 1.0f, EMontagePlayReturnType::MontageLength, _animationFrameToLoad, true);
	}
}

// Doit être implémentée pour chaque classe enfant (serait virtuelle pure si Unreal était pas relou)
void UAttack::StartAttack() {}
void UAttack::StopAttack() {}
void UAttack::OverAttack(){
	attackState = EAttackState::OFF;
}


void UAttack::ApplySaveState(UAttackSaveState _saveState) {
	LoadAttackSaveState(_saveState.attackState_Saved, _saveState.animationFrame_Saved);
}
UAttackSaveState UAttack::GetSaveState() {
	UAttackSaveState _saveState;
	_saveState.attackState_Saved = attackState;
	_saveState.animationFrame_Saved = GetAttackFrame();
	return _saveState;
}
void UAttack::LoadAttackSaveState(EAttackState _stateToLoad, float _animationFrameToLoad) {
	attackState = _stateToLoad;
	PlayMontageFromCurrentState(_animationFrameToLoad);
}
float UAttack::GetAttackFrame() {
	if (attackState != EAttackState::OFF)
		return godAnimInstance->Montage_GetPosition(attackMontage);
	else
		return -1;
}
EAttackState UAttack::GetAttackState() {
	return attackState;
}


void UAttack::OnOverNotify() {
	OverAttack();
}
void UAttack::OnActiveNotify(){}
void UAttack::OnChangeNotify(){}
void UAttack::OnInactiveNotify(){}


void UAttack::SetUpAttack(AGod* _god, int _idAttack) {
	god = _god;
	godAnimInstance = Cast<UGodAnimInstance>(god->GetSkeletalMesh()->GetAnimInstance());
	idAttackOnGodAttackComponent = _idAttack;
}
const EAttackState UAttack::getState() {
	return attackState;
}