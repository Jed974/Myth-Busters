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


void UAttackSingleHit::SpwanHitBoxGroup() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "AttackSingleHit started an attack !");

	FTransform _spawnTransform = god->GetRootComponent()->GetSocketTransform(SocketToAttachTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = god;

	hitBoxGroup = GetWorld()->SpawnActor<AHitBoxGroup>(hitBoxGroupToSpawn, _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

	FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	if (SocketToAttachTo != "NONE")
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules, SocketToAttachTo);
	else
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules);

	hitBoxGroup->facingRight = god->GetGodMovementComponent()->GetIsFacingRight();

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

/*void UAttackSingleHit::ApplySaveState(UAttackSaveState _saveState) {
	Super::ApplySaveState(_saveState);
	if (attackState == EAttackState::HITACTIVE1) {
		SpwanHitBoxGroup();
	}
}*/
void UAttackSingleHit::LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad, bool LoadAnimation) {
	Super::LoadAtAttackStateAndFrame(_stateToLoad, _animationFrameToLoad, LoadAnimation);
	if (attackState == EAttackState::HITACTIVE1) {
		SpwanHitBoxGroup();
	}
}

void UAttackSingleHit::OnActiveNotify() {
	SpwanHitBoxGroup();
}
void UAttackSingleHit::OnInactiveNotify() {
	DestroyHitBoxGroup();
}