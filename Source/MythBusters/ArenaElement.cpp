// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaElement.h"

void UArenaElement::SetId(int i) {
	IdElement = i;
	AddLocalRotation(FQuat(FRotator(i * 10, 0, 0)));
}

int UArenaElement::GetId() {
	return IdElement;
}

void UArenaElement::Explode() {
	AddLocalRotation(FQuat(FRotator(0, 45, 0)));
}