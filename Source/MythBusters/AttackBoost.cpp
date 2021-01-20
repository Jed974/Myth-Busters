// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBoost.h"
#include "god.h"

UAttackBoost::UAttackBoost(){}

void UAttackBoost::StartAttack() {
	attackState = EAttackState::START;
	PlayMontageFromCurrentState(0);
}
void UAttackBoost::StopAttack() {
	attackState = EAttackState::OFF;
}

void UAttackBoost::ApplyBoost() {
	god->GetGodBoostComponent()->TemporaryBoost(boostType, boostPercent, frameDuration);
}

void UAttackBoost::OnActiveNotify() {
	ApplyBoost();
	attackState = EAttackState::RECOVER;
}