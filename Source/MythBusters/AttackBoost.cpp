// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBoost.h"
#include "god.h"

UAttackBoost::UAttackBoost(){}


void UAttackBoost::ApplyBoost() {
	god->GetGodBoostComponent()->TemporaryBoost(boostType, boostPercent, frameDuration);
}

void UAttackBoost::OnActiveNotify() {
	ApplyBoost();
	attackState = EAttackState::RECOVER;
}