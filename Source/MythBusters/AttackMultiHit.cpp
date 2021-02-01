// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMultiHit.h"
#include "god.h"
#include "GodAnimInstance.h"
#include "HitBoxGroup.h"

UAttackMultiHit::UAttackMultiHit() : UAttack() {
}


void UAttackMultiHit::StopAttack() {
	if (attackState != EAttackState::OFF) {
		DestroyHitBoxGroup();
		attackState = EAttackState::OFF;
	}
}


void UAttackMultiHit::SpawnHitBoxGroup(int id, bool alreadyHit) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "AttackSingleHit started an attack !");

	FTransform _spawnTransform = god->GetRootComponent()->GetSocketTransform(SocketToSpawnTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = god;

	hitBoxGroup = GetWorld()->SpawnActor<AHitBoxGroup>(hitBoxGroupsToSpawn[id], _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

	FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	if (SocketToSpawnTo != "NONE")
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules, SocketToSpawnTo);
	else
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules);
	hitBoxGroup->facingRight = god->GetGodMovementComponent()->GetIsFacingRight();	
	hitBoxGroup->alreadyHit = alreadyHit;	


	idHitboxGroup = id;

	switch (idHitboxGroup) {
		case 0 :
			attackState = EAttackState::HITACTIVE1;
			break;
		case 1:
			attackState = EAttackState::HITACTIVE2;
			break;
		default:
			attackState = EAttackState::HITACTIVE3;
			break;
	}
}

void UAttackMultiHit::DestroyHitBoxGroup() {
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

void UAttackMultiHit::ApplySaveState(UAttackSaveState _saveState, bool _playAnimation) {
	Super::ApplySaveState(_saveState, _playAnimation);
	switch (attackState) {
	case EAttackState::HITACTIVE1:
		SpawnHitBoxGroup(0, _saveState.auxAlreadyHit);
		break;
	case EAttackState::HITACTIVE2:
		SpawnHitBoxGroup(1, _saveState.auxAlreadyHit);
		break;
	case EAttackState::HITACTIVE3:
		SpawnHitBoxGroup(2, _saveState.auxAlreadyHit);
		break;
	default:
		break;
	}
}
/*void UAttackMultiHit::LoadAtAttackStateAndFrame(EAttackState _stateToLoad, float _animationFrameToLoad, bool LoadAnimation) {
	Super::LoadAtAttackStateAndFrame(_stateToLoad, _animationFrameToLoad, LoadAnimation);
	switch (attackState) {
	case EAttackState::HITACTIVE1:
		SpwanHitBoxGroup(0);
		break;
	case EAttackState::HITACTIVE2:
		SpwanHitBoxGroup(1);
		break;
	case EAttackState::HITACTIVE3:
		SpwanHitBoxGroup(2);
		break;
	default:
		break;
	}
}*/

void UAttackMultiHit::OnActiveNotify() {
	SpawnHitBoxGroup(0);
}
void UAttackMultiHit::OnChangeNotify() {
	DestroyHitBoxGroup();
	SpawnHitBoxGroup(idHitboxGroup + 1);
}
void UAttackMultiHit::OnInactiveNotify() {
	DestroyHitBoxGroup();
}

