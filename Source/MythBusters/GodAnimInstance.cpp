// Fill out your copyright notice in the Description page of Project Settings.


#include "GodAnimInstance.h"


void UGodAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();

	God = Cast<AGod>(GetOwningActor());
}

void UGodAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (God != nullptr) {
		SpeedX = God->GetAnimValues(0);
		SpeedY = God->GetAnimValues(1);
		if (Ejected) {
			EjectX = God->GetAnimValues(2);
			EjectY = God->GetAnimValues(3);
		}
	}	
}

void UGodAnimInstance::InterruptAttack(bool hurting, bool ejecting) {
	AttCanStop = true;
	AttNormal = false;
	AttSpe = false;
	AttPush = false;

	if (hurting)
		Hurted = true;
	else if (ejecting)
		Ejected = true;

	// TODO :: Ne marche pas toujours... Des fois le groupe reste dans la scène et jsp pourquoi
	if (InterruptableAttGroup != nullptr) {
		bool destroySuccess = InterruptableAttGroup->Destroy();
		if (destroySuccess)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Destroy successful");
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Destroy failed");
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "No group to destroy");
}

void UGodAnimInstance::StartAttackInterruptable(TSubclassOf<AHitBoxGroup> ClassToSpwan, FName SocketToAttachTo) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Attack Started !");

	FTransform _spawnTransform = God->GetRootComponent()->GetSocketTransform(SocketToAttachTo);
	_spawnTransform.SetScale3D(FVector(1, 1, 1));

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = God;

	InterruptableAttGroup = GetWorld()->SpawnActor<AHitBoxGroup>(ClassToSpwan, _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

	FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	InterruptableAttGroup->AttachToComponent(God->GetSkeletalMesh(), _attTransformRules, SocketToAttachTo);
}