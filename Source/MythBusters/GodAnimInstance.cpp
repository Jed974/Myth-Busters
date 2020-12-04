// Fill out your copyright notice in the Description page of Project Settings.


#include "GodAnimInstance.h"



void UGodAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	God = Cast<AGod>(GetOwningActor());

	
}

void UGodAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (God != nullptr) 
	{
		
		GodState = God->State;
		switch (GodState)
		{
		case EGodState::Flying:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Flying"));
			break;
		case EGodState::Ejected:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ejected"));
			EjectX = God->GetAnimValues(2);
			EjectY = God->GetAnimValues(3);
			break;
		case EGodState::Sprinting:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sprinting"));
			break;
		}
		SpeedX = God->GetAnimValues(0);
		SpeedY = God->GetAnimValues(1);
	}	
}

void UGodAnimInstance::InterruptAttack()
{
	AnimCanStop = true;
	AttNormal = false;
	AttSpe = false;
	AttPush = false;

	
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

void UGodAnimInstance::StartAttackInterruptable(TSubclassOf<AHitBoxGroup> ClassToSpwan, FName SocketToAttachTo)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Attack Started !");

	FTransform _spawnTransform = God->GetRootComponent()->GetSocketTransform(SocketToAttachTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = God;

	InterruptableAttGroup = GetWorld()->SpawnActor<AHitBoxGroup>(ClassToSpwan, _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

	FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	InterruptableAttGroup->AttachToComponent(God->GetSkeletalMesh(), _attTransformRules, SocketToAttachTo);
}

void UGodAnimInstance::StartAttackProjectile(TSubclassOf<AHitBoxGroup> ClassToSpwan, FName SocketToAttachTo)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Projectile Launched !");

	FTransform _spawnTransform = God->GetRootComponent()->GetSocketTransform(SocketToAttachTo);

	FActorSpawnParameters _spawnParams;
	_spawnParams.Instigator = God;

	GetWorld()->SpawnActor<AHitBoxGroup>(ClassToSpwan, _spawnTransform.GetLocation(), God->GetSkeletalMesh()->GetRelativeRotation(), _spawnParams);
}