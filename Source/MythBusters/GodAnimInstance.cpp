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