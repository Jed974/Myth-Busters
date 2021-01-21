// Fill out your copyright notice in the Description page of Project Settings.


#include "GodAttackComponent.h"
#include "HitBoxGroupProjectile.h"
#include "AttackProjectile.h"
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

	for (int i = 0; i< 14; i++)
	{
		if (Attacks[i] != nullptr) {
			Attacks[i]->SetUpAttack(_godOwner, i);
		}
	}
}


// Called every frame
void UGodAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (currentAttack >= 0 && Attacks[currentAttack]->getState() == EAttackState::OFF) {
		currentAttack = -1;
		Cast<AGod>(GetOwner())->ChangeGodState(EGodState::Flying);
	}
	CleanUpProjectile();
}

bool UGodAttackComponent::StartNormalAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[0] != nullptr) {
				currentAttack = 0;
				Attacks[0]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[1] != nullptr) {
				currentAttack = 1;
				Attacks[1]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[2] != nullptr) {
				currentAttack = 2;
				Attacks[2]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::UP:
			if (Attacks[3] != nullptr) {
				currentAttack = 3;
				Attacks[3]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::DOWN:
			if (Attacks[4] != nullptr) {
				currentAttack = 4;
				Attacks[4]->StartAttack();
			}
			else
				return false;
			break;
		default:
			break;
		}
		return true;
	}
	else
		return false;
}
bool UGodAttackComponent::StartSpecialAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[5] != nullptr) {
				currentAttack = 5;
				Attacks[5]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[6] != nullptr) {
				currentAttack = 6;
				Attacks[6]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[7] != nullptr) {
				currentAttack = 7;
				Attacks[7]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::UP:
			if (Attacks[8] != nullptr) {
				currentAttack = 8;
				Attacks[8]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::DOWN:
			if (Attacks[9] != nullptr) {
				currentAttack = 9;
				Attacks[9]->StartAttack();
			}
			else
				return false;
			break;
		default:
			break;
		}
		return true;
	}
	else
		return false;
}
bool UGodAttackComponent::StartPushAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[10] != nullptr) {
				currentAttack = 10;
				Attacks[10]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[10] != nullptr) {
				currentAttack = 10;
				Attacks[10]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[11] != nullptr) {
				currentAttack = 11;
				Attacks[11]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::UP:
			if (Attacks[12] != nullptr) {
				currentAttack = 12;
				Attacks[12]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::DOWN:
			if (Attacks[13] != nullptr) {
				currentAttack = 13;
				Attacks[13]->StartAttack();
			}
			else
				return false;
			break;
		default:
			break;
		}
		return true;
	}
	else
		return false;
}
void UGodAttackComponent::InterruptAttack() {
	for (auto& att : Attacks) {
		if (att != nullptr) {
			att->StopAttack();
		}
	}
	currentAttack = -1;
}

bool UGodAttackComponent::GetIsCurrentAttack() const {
	return currentAttack > -1;
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

void UGodAttackComponent::RegisterProjectile(AHitBoxGroupProjectile* _projectile, int _idAttack) {
	if (_projectile != nullptr && _idAttack > -1) {
		if (AllProjectiles.Contains(_idAttack)) {
			// The array for this attack already exists (projectiles already in scene)
			AllProjectiles[_idAttack].Projectiles.Add(_projectile);
		}
		else {
			// Array for this attack doesn't exist yet
			AllProjectiles.Add(_idAttack, FProjectileArray());
			AllProjectiles[_idAttack].Projectiles.Add(_projectile);
		}
	}
}
void UGodAttackComponent::CleanUpProjectile() {
	TArray<int> idArrayToRemove;
	for (auto &pair : AllProjectiles) {
		while (pair.Value.Projectiles.Num() > 0 && !IsValid(pair.Value.Projectiles[0])) {
			pair.Value.Projectiles.RemoveAt(0);
			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Green, "Projectile cleaned");
		}
		if (pair.Value.Projectiles.Num() == 0) {
			// l'index 0 est invalide -> le tableau est vide, on supprime la paire
			idArrayToRemove.Add(pair.Key);
		}
	}
	if (idArrayToRemove.Num() > 0) {
		for (int &id : idArrayToRemove) {
			AllProjectiles.Remove(id);
		}
		AllProjectiles.Compact();
		AllProjectiles.Shrink();
	}
}


FAttacksSaveState UGodAttackComponent::SaveAttacksState() {
	FAttacksSaveState _saveState;

	// Attaque courrante
	if (currentAttack != -1) {
		// id
		_saveState.idCurrentAttack = currentAttack;

		// Récupérer le saveState
		_saveState.montageFrame = Attacks[currentAttack]->GetAttackFrame();
		_saveState.attackState = Attacks[currentAttack]->GetSaveState();
	}

	

	// Projectiles
	for (auto &pair : AllProjectiles) {
		GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Green, "Array de Simplified créé pour l'attaque " + FString::SanitizeFloat(pair.Key) + " contenant " + FString::SanitizeFloat(pair.Value.Projectiles.Num()) + " projectiles.");
		//_saveState.projectiles.Add(pair.Key, USimplifiedProjectileArray());
		_saveState.projectiles.Add(pair.Key, FSimplifiedProjectileArray());

		for (auto &proj : pair.Value.Projectiles) {
			_saveState.projectiles[pair.Key].Projectiles.Add(proj->getSimplifiedVersion());
		}		
	}
	return _saveState;
}
void UGodAttackComponent::LoadAttacksState(FAttacksSaveState saveState) {
	// Cancel all attacks
	for (int i = 0; i < 14; i++) {
		if (Attacks[i] != nullptr) {
			Attacks[i]->StopAttack();
		}
	}

	// Go back to past attack
	currentAttack = saveState.idCurrentAttack;
	if (currentAttack != -1)
		Attacks[currentAttack]->ApplySaveState(saveState.attackState);

	// Projectiles
	for (int i = 0; i < 14; i++) {
		if (AllProjectiles.Contains(i) && !saveState.projectiles.Contains(i)) {
			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Green, "Proj présents mais pas dans le saveState");
			// Currently we have some projectiles but they were not present when state was saved => destroy them
			for (auto &proj : AllProjectiles[i].Projectiles) {
				proj->Destroy();
			}
		}
		else if (!AllProjectiles.Contains(i) && saveState.projectiles.Contains(i)) {
			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Red, "Proj absents mais présents dans le saveState");
			// There were some projectiles in the past but not they are gone => recreate them + a slot for the attack
			UAttackProjectile* attackProjectile = Cast<UAttackProjectile>(Attacks[i]);
			if (attackProjectile == nullptr) {
				GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Red, "Cast de l'attaque a échoué : impossible de recréer les projectiles.");
				return;
			}
			else {
				AllProjectiles.Add(i, FProjectileArray());
				for (auto &simplified : saveState.projectiles[i].Projectiles) {
					AHitBoxGroupProjectile* projectile = attackProjectile->SpwanHitBoxGroup();
					projectile->applySimplifiedVersion(simplified);
					AllProjectiles[i].Projectiles.Add(projectile);
				}
			}
		}
		else if (AllProjectiles.Contains(i) && saveState.projectiles.Contains(i)) {
			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Yellow, "Proj présents dans le savestate et la scene");
			// There are projectiles in game and in save state for the same attack
			if (AllProjectiles[i].Projectiles.Num() > saveState.projectiles[i].Projectiles.Num()) {
				// Il y a plus de projectiles in game que dans le save state : détruire le surplus
				int surplus = FMath::Abs(AllProjectiles[i].Projectiles.Num() - saveState.projectiles[i].Projectiles.Num());
				for (int j = 0; j < surplus; j++) {
					AllProjectiles[i].Projectiles[0]->Destroy();
					AllProjectiles[i].Projectiles.RemoveAt(0);
				}
			}
			else if (AllProjectiles[i].Projectiles.Num() < saveState.projectiles[i].Projectiles.Num()) {
				// Il y a moins de projectiles in game que dans le save state : ajouter le manque
				UAttackProjectile* attackProjectile = Cast<UAttackProjectile>(Attacks[i]);
				if (attackProjectile == nullptr) {
					GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Red, "Cast de l'attaque a échoué : impossible d'ajouter les projectiles manquants.");
					return;
				}
				else {
					int manque = FMath::Abs(saveState.projectiles[i].Projectiles.Num() - AllProjectiles[i].Projectiles.Num());
					for (int j = 0; j < manque; j++) {
						AHitBoxGroupProjectile* projectile = attackProjectile->SpwanHitBoxGroup();
						AllProjectiles[i].Projectiles.Add(projectile);
					}
				}				
			}

			// Ensuite : appliquer les simplified
			for (int j = 0; j < saveState.projectiles[i].Projectiles.Num(); j++) {
				AllProjectiles[i].Projectiles[j]->applySimplifiedVersion(saveState.projectiles[i].Projectiles[j]);
			}
		}
	}

	CleanUpProjectile(); // in case we have destroy some projectiles during rollback
}