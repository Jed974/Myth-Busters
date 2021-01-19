// Fill out your copyright notice in the Description page of Project Settings.


#include "GodBoostComponent.h"

FBoost::FBoost() : frameAlive(0), tempPercent(0), definitivePercent(0) {}


UGodBoostComponent::UGodBoostComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	boosts.Init(FBoost(), 5);
}


void UGodBoostComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGodBoostComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto &boost : boosts) {
		if (boost.frameAlive > 0) {
			boost.frameAlive--;
			if (boost.frameAlive == 0) {
				boost.tempPercent = 0;
			}
		}
	}
}



void UGodBoostComponent::TemporaryBoost(EBoostType type, int percent, int durationFrames) {
	boosts[(int)type].tempPercent = percent;
	boosts[(int)type].frameAlive = durationFrames;
}
void UGodBoostComponent::GameBoost(EBoostType type, int percent) {
	boosts[(int)type].definitivePercent = percent;
}

float UGodBoostComponent::GetBoost(EBoostType type) {
	int percentBoost = boosts[(int)type].definitivePercent + (boosts[(int)type].frameAlive > 0 ? boosts[(int)type].tempPercent : 0);
	return std::max(1 + ((float)percentBoost) / 100, 0.0f);
}

TArray<FBoost> UGodBoostComponent::GetBoostSaveState() const {
	return boosts;
}
void UGodBoostComponent::SetBoostSaveState(TArray<FBoost> savedBoosts) {
	boosts = savedBoosts;
}