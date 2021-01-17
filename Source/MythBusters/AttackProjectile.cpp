// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackProjectile.h"
#include "god.h"
#include "GodAnimInstance.h"
#include "HitBoxGroupProjectile.h"

UAttackProjectile::UAttackProjectile() : UAttack() {
}


void UAttackProjectile::StartAttack() {
	attackState = EAttackState::START;
	PlayMontageFromCurrentState(0);
}

void UAttackProjectile::StopAttack() {
	attackState = EAttackState::OFF;
}


AHitBoxGroupProjectile* UAttackProjectile::SpwanHitBoxGroup() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Projectile Launched !");
	FTransform _spawnTransform = god->GetRootComponent()->GetSocketTransform(SocketToAttachTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = god;

	AHitBoxGroupProjectile* proj = GetWorld()->SpawnActor<AHitBoxGroupProjectile>(projectileToSpawn, _spawnTransform.GetLocation(), god->GetSkeletalMesh()->GetRelativeRotation(), _spawnParams);
	proj->facingRight = god->GetGodMovementComponent()->GetIsFacingRight();
	
	return proj;
}


/*void UAttackProjectile::DestroyAllProjectiles() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Destoy all " + FString::SanitizeFloat(Projectiles.Num()));
	for (auto &proj : Projectiles) {
		bool destroySuccess = proj->Destroy();
		if (destroySuccess)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Destroy successful");
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Destroy failed");
		proj = nullptr;
	}
	Projectiles.Empty();
}*/


void UAttackProjectile::OnActiveNotify() {
	god->RegisterProjectile(SpwanHitBoxGroup(), idAttackOnGodAttackComponent);
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