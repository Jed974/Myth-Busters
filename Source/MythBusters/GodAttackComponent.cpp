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
	Attacks[0] = Att_Normal_Neutral.GetDefaultObject();
	Attacks[1] = Att_Normal_Forward.GetDefaultObject();
	Attacks[2] = Att_Normal_Backward.GetDefaultObject();
	Attacks[3] = Att_Normal_Up.GetDefaultObject();
	Attacks[4] = Att_Normal_Down.GetDefaultObject();
	Attacks[5] = Att_Special_Neutral.GetDefaultObject();
	Attacks[6] = Att_Special_Forward.GetDefaultObject();
	Attacks[7] = Att_Special_Backward.GetDefaultObject();
	Attacks[8] = Att_Special_Up.GetDefaultObject();
	Attacks[9] = Att_Special_Down.GetDefaultObject();
	Attacks[10] = Att_Push_Forward.GetDefaultObject();
	Attacks[11] = Att_Push_Backward.GetDefaultObject();
	Attacks[12] = Att_Push_Up.GetDefaultObject();
	Attacks[13] = Att_Push_Down.GetDefaultObject();	

	AGod* _godOwner = Cast<AGod>(GetOwner());
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
			currentAttack = 0;
			Attacks[0]->StartAttack();
			break;
		case EAttackDirection::FORWARD:
			currentAttack = 1;
			Attacks[1]->StartAttack();
			break;
		case EAttackDirection::BACKWARD:
			currentAttack = 2;
			Attacks[2]->StartAttack();
			break;
		case EAttackDirection::UP:
			currentAttack = 3;
			Attacks[3]->StartAttack();
			break;
		case EAttackDirection::DOWN:
			currentAttack = 4;
			Attacks[4]->StartAttack();
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
			currentAttack = 5;
			Attacks[5]->StartAttack();
			break;
		case EAttackDirection::FORWARD:
			currentAttack = 6;
			Attacks[6]->StartAttack();
			break;
		case EAttackDirection::BACKWARD:
			currentAttack = 7;
			Attacks[7]->StartAttack();
			break;
		case EAttackDirection::UP:
			currentAttack = 8;
			Attacks[8]->StartAttack();
			break;
		case EAttackDirection::DOWN:
			currentAttack = 9;
			Attacks[9]->StartAttack();
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
			currentAttack = 10;
			Attacks[10]->StartAttack();
			break;
		case EAttackDirection::FORWARD:
			currentAttack = 10;
			Attacks[10]->StartAttack();
			break;
		case EAttackDirection::BACKWARD:
			currentAttack = 11;
			Attacks[11]->StartAttack();
			break;
		case EAttackDirection::UP:
			currentAttack = 12;
			Attacks[12]->StartAttack();
			break;
		case EAttackDirection::DOWN:
			currentAttack = 13;
			Attacks[13]->StartAttack();
			break;
		default:
			break;
		}
	}
}

void UGodAttackComponent::TransmitNotify(ENotifyType _notifyType) {
	if (currentAttack >= 0) {
		switch (_notifyType) {
			{
		case ENotifyType::OVER:
			Attacks[currentAttack]->OnOverNotify();
			break;
		case ENotifyType::ACTIVE:
			Attacks[currentAttack]->OnActiveNotify();
			break;
		case ENotifyType::CHANGE:
			Attacks[currentAttack]->OnChangeNotify();
			break;
		case ENotifyType::INACTIVE:
			Attacks[currentAttack]->OnInactiveNotify();
			break;
		default:
			break;
			}
		}
	}
}