// Fill out your copyright notice in the Description page of Project Settings.


#include "GodShieldComponent.h"
#include "god.h"

UGodShieldComponent::UGodShieldComponent(){
	godOwner = Cast<AGod>(GetOwner());
	PrimaryComponentTick.bCanEverTick = true;
}

void UGodShieldComponent::BeginPlay()
{
	Super::BeginPlay();
	DELTA_TIME = 1.0 / GEngine->FixedFrameRate;
}
// Called every frame
void UGodShieldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (shieldLifeTime > 0) {
		shieldLifeTime -= (DELTA_TIME * godOwner->GetGodBoostComponent()->GetBoost(EBoostType::SHIELD))/2;
		if (shieldLifeTime < 0) {
			shieldLifeTime = 0;
		}
	}
}

bool UGodShieldComponent::StartShield() { 
	if (CurrentShield != nullptr)
		return false;
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Shielding !");

		FTransform _spawnTransform = godOwner->GetRootComponent()->GetComponentTransform();
		FActorSpawnParameters _spawnParams;
		_spawnParams.Instigator = godOwner;

		CurrentShield = GetWorld()->SpawnActor<AShield>(ShieldClassToSpwan, _spawnTransform.GetLocation(), _spawnTransform.GetRotation().Rotator(), _spawnParams);

		FAttachmentTransformRules _attTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		CurrentShield->AttachToComponent(godOwner->CapsuleComponent, _attTransformRules);
		CurrentShield->InitShield(ShieldSize, shieldLifeTime, ShieldFresnelColor, ShieldBaseColor);
		return true;
	}
}
void UGodShieldComponent::StopShield(){
	if (CurrentShield != nullptr)
	{
		shieldLifeTime = CurrentShield->GetLifeTime();
		CurrentShield->Destroy();
		CurrentShield = nullptr;
	}
}

void UGodShieldComponent::OrientShieldX(float AxisValue) {
	CurrentShield->SetInputDirectionVectorX(AxisValue);
}
void UGodShieldComponent::OrientShieldY(float AxisValue) {
	CurrentShield->SetInputDirectionVectorY(AxisValue * -1);
}