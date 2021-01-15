#include "Attack.h"
#include "god.h"
#include "GodAnimInstance.h"


UAttack::UAttack() {
	attackState = EAttackState::OFF;
}

UAttack::~UAttack() {}


void UAttack::PlayMontageFromCurrentState(int _animationFrameToLoad) {
	if (attackState != EAttackState::OFF) {
		godAnimInstance->Montage_Play(attackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0, true); // 0 -> _animationFrameToLoad / 60
		// TODO : Modifier l'encroit où on commence le montage !!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
}

// Doit être implémentée pour chaque classe enfant (serait virtuelle pure si Unreal était pas relou)
void UAttack::StartAttack() {}
void UAttack::StopAttack() {}
void UAttack::OverAttack(){
	attackState = EAttackState::OFF;
}


void UAttack::LoadAttackSaveState(EAttackState* _stateToLoad, int _animationFrameToLoad) {
	attackState = *_stateToLoad;
	if (_animationFrameToLoad >= 0)
		PlayMontageFromCurrentState(_animationFrameToLoad);
}



void UAttack::OnOverNotify() {
	OverAttack();
}
void UAttack::OnActiveNotify(){}
void UAttack::OnChangeNotify(){}
void UAttack::OnInactiveNotify(){}


void UAttack::SetGod(AGod* _god) {
	god = _god;
	godAnimInstance = Cast<UGodAnimInstance>(god->GetSkeletalMesh()->GetAnimInstance());
}
const EAttackState UAttack::getState() {
	return attackState;
}