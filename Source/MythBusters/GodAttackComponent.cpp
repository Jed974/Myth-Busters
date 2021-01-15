// Fill out your copyright notice in the Description page of Project Settings.


#include "GodAttackComponent.h"
#include "god.h"

// Sets default values for this component's properties
UGodAttackComponent::UGodAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGodAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Attacks.Init(nullptr, 14);
	Attacks[0] = Att_Normal_Neutral != nullptr ? NewObject<UAttack>(this, *Att_Normal_Neutral) : nullptr;
	Attacks[1] = Att_Normal_Forward != nullptr ? NewObject<UAttack>(this, *Att_Normal_Forward) : nullptr;
	Attacks[2] = Att_Normal_Backward != nullptr ? NewObject<UAttack>(this, *Att_Normal_Backward) : nullptr;
	Attacks[3] = Att_Normal_Up != nullptr ? NewObject<UAttack>(this, *Att_Normal_Up) : nullptr;
	Attacks[4] = Att_Normal_Down != nullptr ? NewObject<UAttack>(this, *Att_Normal_Down) : nullptr;
	Attacks[5] = Att_Special_Neutral != nullptr ? NewObject<UAttack>(this, *Att_Special_Neutral) : nullptr;
	Attacks[6] = Att_Special_Forward != nullptr ? NewObject<UAttack>(this, *Att_Special_Forward) : nullptr;
	Attacks[7] = Att_Special_Backward != nullptr ? NewObject<UAttack>(this, *Att_Special_Backward) : nullptr;
	Attacks[8] = Att_Special_Up != nullptr ? NewObject<UAttack>(this, *Att_Special_Up) : nullptr;
	Attacks[9] = Att_Special_Down != nullptr ? NewObject<UAttack>(this, *Att_Special_Down) : nullptr;
	Attacks[10] = Att_Push_Forward != nullptr ? NewObject<UAttack>(this, *Att_Push_Forward) : nullptr;
	Attacks[11] = Att_Push_Backward != nullptr ? NewObject<UAttack>(this, *Att_Push_Backward) : nullptr;
	Attacks[12] = Att_Push_Up != nullptr ? NewObject<UAttack>(this, *Att_Push_Up) : nullptr;
	Attacks[13] = Att_Push_Down != nullptr ? NewObject<UAttack>(this, *Att_Push_Down) : nullptr;

	AGod* _godOwner = Cast<AGod>(GetOwner());
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, _godOwner);

	for (auto &att : Attacks)
	{
		if (att != nullptr) {
			att->SetGod(_godOwner);
		}
	}
}


// Called every frame
void UGodAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (currentAttack >= 0 && Attacks[currentAttack]->getState() == EAttackState::OFF) {
		currentAttack = -1;
	}
}



void UGodAttackComponent::StartNormalAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[0] != nullptr) {
				currentAttack = 0;
				Attacks[0]->StartAttack();
			}
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[1] != nullptr) {
				currentAttack = 1;
				Attacks[1]->StartAttack();
			}
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[2] != nullptr) {
				currentAttack = 2;
				Attacks[2]->StartAttack();
			}
			break;
		case EAttackDirection::UP:
			if (Attacks[3] != nullptr) {
				currentAttack = 3;
				Attacks[3]->StartAttack();
			}
			break;
		case EAttackDirection::DOWN:
			if (Attacks[4] != nullptr) {
				currentAttack = 4;
				Attacks[4]->StartAttack();
			}
			break;
		default:
			break;
		}
	}
}
void UGodAttackComponent::StartSpecialAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[5] != nullptr) {
				currentAttack = 5;
				Attacks[5]->StartAttack();
			}
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[6] != nullptr) {
				currentAttack = 6;
				Attacks[6]->StartAttack();
			}
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[7] != nullptr) {
				currentAttack = 7;
				Attacks[7]->StartAttack();
			}
			break;
		case EAttackDirection::UP:
			if (Attacks[8] != nullptr) {
				currentAttack = 8;
				Attacks[8]->StartAttack();
			}
			break;
		case EAttackDirection::DOWN:
			if (Attacks[9] != nullptr) {
				currentAttack = 9;
				Attacks[9]->StartAttack();
			}
			break;
		default:
			break;
		}
	}
}
void UGodAttackComponent::StartPushAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[10] != nullptr) {
				currentAttack = 10;
				Attacks[10]->StartAttack();
			}
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[10] != nullptr) {
				currentAttack = 10;
				Attacks[10]->StartAttack();
			}
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[11] != nullptr) {
				currentAttack = 11;
				Attacks[11]->StartAttack();
			}
			break;
		case EAttackDirection::UP:
			if (Attacks[12] != nullptr) {
				currentAttack = 12;
				Attacks[12]->StartAttack();
			}
			break;
		case EAttackDirection::DOWN:
			if (Attacks[13] != nullptr) {
				currentAttack = 13;
				Attacks[13]->StartAttack();
			}
			break;
		default:
			break;
		}
	}
}

void UGodAttackComponent::TransmitNotify(ENotifyType _notifyType) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "Notify avec l'atk : " + FString::SanitizeFloat(currentAttack));
	if (currentAttack >= 0) {
		switch (_notifyType) 
		{
		case ENotifyType::OVER:
			Attacks[currentAttack]->OnOverNotify();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Notify over");
			currentAttack = -1;
			break;
		case ENotifyType::ACTIVE:
			Attacks[currentAttack]->OnActiveNotify();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Notify active");
			break;
		case ENotifyType::CHANGE:
			Attacks[currentAttack]->OnChangeNotify();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Notify change");
			break;
		case ENotifyType::INACTIVE:
			Attacks[currentAttack]->OnInactiveNotify();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, "Notify inactive");
			break;
		default:
			break;	
		}
	}
}