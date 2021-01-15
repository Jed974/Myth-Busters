#include "Attack.h"
#include "god.h"
#include "GodAnimInstance.h"


UAttack::UAttack() {
	attackState = EAttackState::OFF;
}

UAttack::~UAttack() {}


void UAttack::PlayMontageFromCurrentState(int _animationFrameToLoad) {
	if (attackState != EAttackState::OFF) {
		godAnimInstance->Montage_Play(attackMontage, 1.0f, EMontagePlayReturnType::MontageLength, _animationFrameToLoad / 60, true);	// TODO : Modifier l'encroit o� on commence le montage !!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
}

// Doit �tre impl�ment�e pour chaque classe enfant (serait virtuelle pure si Unreal �tait pas relou)
void UAttack::StartAttack() {}
void UAttack::StopAttack() {}
void UAttack::EndOfRecoverAttack(){
	attackState = EAttackState::OFF;
}


void UAttack::LoadAttackSaveState(EAttackState* _stateToLoad, int _animationFrameToLoad) {
	attackState = *_stateToLoad;
	if (_animationFrameToLoad >= 0)
		PlayMontageFromCurrentState(_animationFrameToLoad);
}

const bool UAttack::GetAttackOrientation() {
	return true;
}


void UAttack::OnOverNotify() {
	EndOfRecoverAttack();
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