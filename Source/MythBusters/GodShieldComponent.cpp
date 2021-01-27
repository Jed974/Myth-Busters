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


void UGodShieldComponent::SetNoneCustomColors(int idPlayer) {
	switch (idPlayer) {
		case 1:
			ShieldBaseColor = FColor::FromHex("0061FFFF"); //FColor(0,0.5,1,1);
			ShieldFresnelColor = FColor::FromHex("FFBC00FF"); //FColor(0.15, 0, 1, 1);
			break;
		case 2:
			ShieldBaseColor = FColor::FromHex("DB0013FF"); //FColor(1, 0, 1, 1);
			ShieldFresnelColor = FColor::FromHex("0072FFFF"); //FColor(0.7, 0, 1, 1);
			break;
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

FShieldSaveState UGodShieldComponent::SaveShieldState() {
	FShieldSaveState ShSvSt;
	ShSvSt.out = CurrentShield != nullptr;
	ShSvSt.lifeTime = ShSvSt.out ? CurrentShield->GetLifeTime() : shieldLifeTime;
	ShSvSt.angle = ShSvSt.out ? CurrentShield->GetAngle() : 0;
	return ShSvSt;
}
void UGodShieldComponent::LoadShieldState(FShieldSaveState &ShSvSt) {
	if (ShSvSt.out) {
		if (CurrentShield == nullptr)
			StartShield();
		CurrentShield->SetLifeTime(ShSvSt.lifeTime);
		CurrentShield->SetAngle(ShSvSt.angle);
	}
	else {
		if (CurrentShield != nullptr)
			StopShield();
		shieldLifeTime = ShSvSt.lifeTime;
	}
}