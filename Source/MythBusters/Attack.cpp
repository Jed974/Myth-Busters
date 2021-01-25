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
void UAttack::StartAttack() {
	attackState = EAttackState::START;
	curveAbscissa = 0;
	PlayMontageFromCurrentState(0);
}
void UAttack::StopAttack() {
	attackState = EAttackState::OFF;
}
void UAttack::OverAttack(){
	if (cooldDownAmount > 0) {
		cooldDownFrame = cooldDownAmount;
		attackState = EAttackState::COOLDOWN;
	}
	else
		attackState = EAttackState::OFF;
}


void UAttack::ApplySaveState(UAttackSaveState _saveState) {
	curveAbscissa = _saveState.inducedMovementAbscissa;
	LoadAtAttackStateAndFrame(_saveState.attackState_Saved, _saveState.animationFrame_Saved);
}
UAttackSaveState UAttack::GetSaveState() {
	UAttackSaveState _saveState;
	_saveState.attackState_Saved = attackState;
	_saveState.animationFrame_Saved = GetAttackFrame();
	_saveState.inducedMovementAbscissa = curveAbscissa;
	return _saveState;
}
void UAttack::LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad, bool LoadAnimation) {
	attackState = _stateToLoad;
	if (LoadAnimation)
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
EAttackState UAttack::GAS() {
	return attackState;
}


void UAttack::OnOverNotify() {
	OverAttack();
}
void UAttack::OnActiveNotify(){}
void UAttack::OnChangeNotify(){}
void UAttack::OnInactiveNotify(){}
void UAttack::OnNextNotify() {}


void UAttack::SetUpAttack(AGod* _god, int _idAttack, int _auxInfo) {
	god = _god;
	godAnimInstance = Cast<UGodAnimInstance>(god->GetSkeletalMesh()->GetAnimInstance());
	idAttackOnGodAttackComponent = _idAttack;
	auxiliaryInfo = _auxInfo;
	attackState = EAttackState::OFF;
}
void UAttack::DecreaseCoolDown() {
	cooldDownFrame--;
	if (cooldDownFrame <= 0) {
		cooldDownFrame = 0;
		attackState = EAttackState::OFF;
	}
}
int UAttack::GetCoolDown() const {
	return cooldDownFrame;
}
void UAttack::SetCoolDown(int _cd) {
	cooldDownFrame = _cd;
	attackState = EAttackState::COOLDOWN;
}


FVector2D UAttack::GetInducedMovement() {
	if (InducedMovement != nullptr) {
		FVector v3 = InducedMovement->GetVectorValue((float)curveAbscissa / 60);
		if (!god->GetGodMovementComponent()->GetIsFacingRight())
			v3.X *= -1;
		curveAbscissa++;
		return FVector2D(v3.X, v3.Y);
	}
	else
		return FVector2D();
}