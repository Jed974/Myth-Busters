// Fill out your copyright notice in the Description page of Project Settings.


#include "GodAttackComponent.h"
#include "HitBoxGroupProjectile.h"
#include "AttackProjectile.h"
#include "AttackSequencial.h"
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
	if (currentAttack >= 0) {
		if (Attacks[currentAttack]->GetAttackState() == EAttackState::OFF) {
			currentAttack = -1;
			Cast<AGod>(GetOwner())->ChangeGodState(EGodState::Flying);
		}
		else {
			FVector2D induced = Attacks[currentAttack]->GetInducedMovement();
			if (induced.X  > -2)
				Cast<AGod>(GetOwner())->GetGodMovementComponent()->AddMovementInput(induced, 1.0);
		}
	}

	// CoolDowns
	for (auto& att : Attacks) {
		if (att != nullptr && att->GetAttackState() == EAttackState::COOLDOWN) {
			att->DecreaseCoolDown();
		}
	}
	CleanUpProjectile();
}

bool UGodAttackComponent::StartNormalAttack(EAttackDirection _attackDirection) {
	if (currentAttack == -1) {
		switch (_attackDirection)
		{
		case EAttackDirection::NEUTRAL:
			if (Attacks[0] != nullptr && Attacks[0]->GetAttackState()==EAttackState::OFF) {	// If we want to be able to cancel attack during StartUp phase, do "or state==Startup"
				currentAttack = 0;
				Attacks[0]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[1] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 1;
				Attacks[1]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[2] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 2;
				Attacks[2]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::UP:
			if (Attacks[3] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 3;
				Attacks[3]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::DOWN:
			if (Attacks[4] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
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
			if (Attacks[5] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 5;
				Attacks[5]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[6] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 6;
				Attacks[6]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[7] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 7;
				Attacks[7]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::UP:
			if (Attacks[8] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 8;
				Attacks[8]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::DOWN:
			if (Attacks[9] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
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
			if (Attacks[10] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 10;
				Attacks[10]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::FORWARD:
			if (Attacks[10] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 10;
				Attacks[10]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::BACKWARD:
			if (Attacks[11] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 11;
				Attacks[11]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::UP:
			if (Attacks[12] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
				currentAttack = 12;
				Attacks[12]->StartAttack();
			}
			else
				return false;
			break;
		case EAttackDirection::DOWN:
			if (Attacks[13] != nullptr && Attacks[0]->GetAttackState() == EAttackState::OFF) {
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
			currentAttack = -1;
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
		case ENotifyType::NEXT:
			Attacks[currentAttack]->OnNextNotify();
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
		_saveState.idCurrentSubAttack = currentAttack;

		// Récupérer le saveState
		_saveState.montageFrame = Attacks[currentAttack]->GetAttackFrame();
		_saveState.attackState = Attacks[currentAttack]->GetSaveState();
	}

	
	//CoolDowns
	_saveState.coolDowns.Init(0,14);
	for (int i = 0; i < 14; i++) {
		if (Attacks[i] != nullptr && Attacks[i]->GetAttackState()== EAttackState::COOLDOWN)
			_saveState.coolDowns[i] = Attacks[i]->GetCoolDown();
	}


	// Projectiles
	for (auto& pair : AllProjectiles) {
		GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Green, "Array de Simplified créé pour l'attaque " + FString::SanitizeFloat(pair.Key) + " contenant " + FString::SanitizeFloat(pair.Value.Projectiles.Num()) + " projectiles.");
		//_saveState.projectiles.Add(pair.Key, USimplifiedProjectileArray());
		_saveState.projectiles.Add(pair.Key, FSimplifiedProjectileArray());

		for (auto& proj : pair.Value.Projectiles) {
			_saveState.projectiles[pair.Key].Projectiles.Add(proj->getSimplifiedVersion());
		}
	}
	return _saveState;
}
void UGodAttackComponent::LoadAttacksState(FAttacksSaveState saveState) {
	// Cancel all attacks + reload cooldowns
	for (int i = 0; i < 14; i++) {
		if (Attacks[i] != nullptr) {
			Attacks[i]->StopAttack();
			if (saveState.coolDowns[i] != 0) {
				Attacks[i]->SetCoolDown(saveState.coolDowns[i]);
			}
		}
	}

	// Go back to past attack
	currentAttack = saveState.idCurrentSubAttack;
	if (currentAttack != -1)
		Attacks[currentAttack]->ApplySaveState(saveState.attackState);

	// Projectiles
	for (int i = 0; i < 14; i++) {
		// Si c'est une attaque de projectile
		UAttackProjectile* attackProjectile = Cast<UAttackProjectile>(Attacks[i]);
		if (attackProjectile != nullptr) {
			if (AllProjectiles.Contains(i) && !saveState.projectiles.Contains(i)) {
				// Currently we have some projectiles but they were not present when state was saved => destroy them
				for (auto& proj : AllProjectiles[i].Projectiles) {
					proj->Destroy();
				}
			}
			else if (!AllProjectiles.Contains(i) && saveState.projectiles.Contains(i)) {
				// We had some projectiles but thez are all gone now => recreate them
				AllProjectiles.Add(i, FProjectileArray());
				for (auto& simplified : saveState.projectiles[i].Projectiles) {
					AHitBoxGroupProjectile* projectile = attackProjectile->SpwanHitBoxGroup();
					projectile->applySimplifiedVersion(simplified);
					AllProjectiles[i].Projectiles.Add(projectile);
				}
			}
			else if (AllProjectiles.Contains(i) && saveState.projectiles.Contains(i)) {
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
					int manque = FMath::Abs(saveState.projectiles[i].Projectiles.Num() - AllProjectiles[i].Projectiles.Num());
					for (int j = 0; j < manque; j++) {
						AHitBoxGroupProjectile* projectile = attackProjectile->SpwanHitBoxGroup();
						AllProjectiles[i].Projectiles.Add(projectile);
					}
				}

				// Ensuite : appliquer les simplified
				for (int j = 0; j < saveState.projectiles[i].Projectiles.Num(); j++) {
					AllProjectiles[i].Projectiles[j]->applySimplifiedVersion(saveState.projectiles[i].Projectiles[j]);
				}
			}
		}
		else {
			// Si c'est une att sequentielle
			UAttackSequencial* attackSeq = Cast<UAttackSequencial>(Attacks[i]);
			if (attackSeq != nullptr) {
				// Check if there is the same amount of projectiles + all projectiles have same auxInfo (bool method)
					// just apply all simplified
				if (!TryApplySequentialProjectileArray(i, saveState)) {
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Try False ! ");
					// previous method failed
					// Les projectiles pouvant être issus de différentes Attack Proj, le + simple est de tous les détruire et refaire
					if (AllProjectiles.Contains(i)) {
						// Currently we have some projectiles => destroy them
						for (auto& proj : AllProjectiles[i].Projectiles) {
							if (proj != nullptr)
								proj->Destroy();
						}
						AllProjectiles.Empty();
					}

					if (saveState.projectiles.Contains(i)) {
						// They were projectiles 
						AllProjectiles.Add(i, FProjectileArray());
						UAttackProjectile* subProjectileAttack = nullptr;
						for (auto& simpProj : saveState.projectiles[i].Projectiles) {
							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(i) + " - " + FString::SanitizeFloat(simpProj.auxiliaryInfo));
							if (attackSeq->GetProjectileAttack(simpProj.auxiliaryInfo, subProjectileAttack)) {
								//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(i) + " - " + FString::SanitizeFloat(subProjectileAttack->GetAuxInfo()));
								AHitBoxGroupProjectile* projectile = subProjectileAttack->SpwanHitBoxGroup();
								projectile->applySimplifiedVersion(simpProj);
								AllProjectiles[i].Projectiles.Add(projectile);
							}
						}
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Contains " +FString::SanitizeFloat(AllProjectiles[i].Projectiles.Num()));
					}
				}
				/*else {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Try true ! ");
				}*/
			}
		}
	}

	CleanUpProjectile(); // in case we have destroy some projectiles during rollback
}
bool UGodAttackComponent::TryApplySequentialProjectileArray(int idAtt, FAttacksSaveState& saveState) {
	if (!AllProjectiles.Contains(idAtt) || !saveState.projectiles.Contains(idAtt))
		return false;	// there is no projectiles in the save state or in the scene
	else if (AllProjectiles[idAtt].Projectiles.Num() != saveState.projectiles[idAtt].Projectiles.Num())
		return false;	// there is not the same amount of projectiles registered in the save state on in the scene
	else
		for (int j = 0; j < saveState.projectiles[idAtt].Projectiles.Num(); j++) {
			if (!IsValid(AllProjectiles[idAtt].Projectiles[j]) && AllProjectiles[idAtt].Projectiles[j]->auxiliaryInfo == saveState.projectiles[idAtt].Projectiles[j].auxiliaryInfo)
				// the projectile number j exists and was spawned by the same attack than the simpProj number (=> same class)
				AllProjectiles[idAtt].Projectiles[j]->applySimplifiedVersion(saveState.projectiles[idAtt].Projectiles[j]);
			else
				return false;
		}
		return true;
}