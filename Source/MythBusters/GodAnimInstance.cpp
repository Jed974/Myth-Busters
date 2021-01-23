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
		GodState = God->State;	// keep track of godstate in class attributes to use it for transitions in machine state
		switch (GodState)
		{
		case EGodState::Flying:
			SpeedX = God->GetAnimValues(0);
			SpeedY = God->GetAnimValues(1);
			break;
		case EGodState::Ejected:
			EjectX = God->GetAnimValues(2);
			EjectY = God->GetAnimValues(3);
			break;
		case EGodState::Sprinting:
			SpeedX = God->GetAnimValues(0);
			SpeedY = God->GetAnimValues(1);
			break;
		}
	}	
}


void UGodAnimInstance::HandleAttackNotify(ENotifyType notifyType) {
	God->HandleAttackNotify(notifyType);
}


void UGodAnimInstance::InterruptAllMontages(){
	Montage_Stop(0, nullptr);
}