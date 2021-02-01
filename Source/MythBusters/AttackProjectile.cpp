// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackProjectile.h"
#include "god.h"
#include "GodAnimInstance.h"
#include "HitBoxGroupProjectile.h"

UAttackProjectile::UAttackProjectile() : UAttack() {
}





AHitBoxGroupProjectile* UAttackProjectile::SpawnHitBoxGroup() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Projectile Launched !");
	FTransform _spawnTransform = god->GetRootComponent()->GetSocketTransform(SocketToSpawnTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = god;

	AHitBoxGroupProjectile* proj = GetWorld()->SpawnActor<AHitBoxGroupProjectile>(projectileToSpawn, _spawnTransform.GetLocation(), god->GetSkeletalMesh()->GetRelativeRotation(), _spawnParams);
	proj->facingRight = god->GetGodMovementComponent()->GetIsFacingRight();
	proj->auxiliaryInfo = auxiliaryInfo;
	
	return proj;
}

void UAttackProjectile::OnActiveNotify() {
	god->RegisterProjectile(SpawnHitBoxGroup(), idAttackOnGodAttackComponent);
	attackState = EAttackState::RECOVER;
}

/*
// ===================== TICK Methods =======================

void UAttackProjectile::Tick(float DeltaTime)
{
	CleanUpProjectiles();
}
bool UAttackProjectile::IsTickable() const { return true; }
bool UAttackProjectile::IsTickableInEditor() const { return false; }
bool UAttackProjectile::IsTickableWhenPaused() const { return false; }
TStatId UAttackProjectile::GetStatId() const { return TStatId(); }
UWorld* UAttackProjectile::GetWorld() const { return GetOuter()->GetWorld(); }
*/