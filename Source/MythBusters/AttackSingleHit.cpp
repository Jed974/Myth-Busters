// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSingleHit.h"
#include "god.h"
#include "GodAnimInstance.h"
#include "HitBoxGroup.h"

UAttackSingleHit::UAttackSingleHit() : UAttack(){
}


void UAttackSingleHit::StopAttack() {
	if (attackState != EAttackState::OFF) {
		DestroyHitBoxGroup();
		attackState = EAttackState::OFF;
	}
}


void UAttackSingleHit::SpawnHitBoxGroup(bool alreadyHit) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "AttackSingleHit started an attack !");

	FTransform _spawnTransform = god->GetRootComponent()->GetSocketTransform(SocketToSpawnTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = god;

	hitBoxGroup = GetWorld()->SpawnActor<AHitBoxGroup>(hitBoxGroupToSpawn, _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

	FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	if (SocketToSpawnTo != "NONE")
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules, SocketToSpawnTo);
	else
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules);

	hitBoxGroup->facingRight = god->GetGodMovementComponent()->GetIsFacingRight();
	hitBoxGroup->alreadyHit = alreadyHit;

	attackState = EAttackState::HITACTIVE1;
}

void UAttackSingleHit::DestroyHitBoxGroup() {
	if (hitBoxGroup != nullptr) {
		bool destroySuccess = hitBoxGroup->Destroy();
		if (destroySuccess)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Destroy successful");
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Destroy failed");
		hitBoxGroup = nullptr;

		attackState = EAttackState::RECOVER;
	}
}

void UAttackSingleHit::ApplySaveState(UAttackSaveState _saveState, bool _playAnimation) {
	Super::ApplySaveState(_saveState, _playAnimation);
	if (attackState == EAttackState::HITACTIVE1) {
		SpawnHitBoxGroup(_saveState.auxAlreadyHit);
	}
}
/*void UAttackSingleHit::LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad, bool LoadAnimation) {
	Super::LoadAtAttackStateAndFrame(_stateToLoad, _animationFrameToLoad, LoadAnimation);
	if (attackState == EAttackState::HITACTIVE1) {
		SpwanHitBoxGroup();
	}
}*/

void UAttackSingleHit::OnActiveNotify() {
	SpawnHitBoxGroup();
}
void UAttackSingleHit::OnInactiveNotify() {
	DestroyHitBoxGroup();
}