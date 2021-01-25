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


void UAttackMultiHit::SpwanHitBoxGroup(int id) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "AttackSingleHit started an attack !");

	FTransform _spawnTransform = god->GetRootComponent()->GetSocketTransform(SocketToAttachTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = god;

	hitBoxGroup = GetWorld()->SpawnActor<AHitBoxGroup>(hitBoxGroupsToSpawn[id], _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

	FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	if (SocketToAttachTo != "NONE")
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules, SocketToAttachTo);
	else
		hitBoxGroup->AttachToComponent(god->GetSkeletalMesh(), _attTransformRules);
	hitBoxGroup->facingRight = god->GetGodMovementComponent()->GetIsFacingRight();
	
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

void UAttackMultiHit::ApplySaveState(UAttackSaveState _saveState) {
	Super::ApplySaveState(_saveState);
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
}

void UAttackMultiHit::OnActiveNotify() {
	SpwanHitBoxGroup(0);
}
void UAttackMultiHit::OnChangeNotify() {
	DestroyHitBoxGroup();
	SpwanHitBoxGroup(idHitboxGroup + 1);
}
void UAttackMultiHit::OnInactiveNotify() {
	DestroyHitBoxGroup();
}

