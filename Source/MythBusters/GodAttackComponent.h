// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//struct FSimplifiedProjectile;
//class USimplifiedProjectileArray;
class AHitBoxGroupProjectile; 
#include "SimplifiedProjectile.h"
#include "CoreMinimal.h"
#include "Attack.h"
#include "Components/ActorComponent.h"
#include "GodAttackComponent.generated.h"

/// Type of AnimNotify that can be trasmitted to attacks from GodAnimInstance by God then GodAttackComponent
UENUM(BlueprintType)
enum class ENotifyType : uint8 {
	OVER,
	ACTIVE,
	CHANGE,
	INACTIVE,
	NEXT
};
/// Attack direction
UENUM(BlueprintType)
enum class EAttackDirection : uint8 {
	NEUTRAL,
	FORWARD,
	BACKWARD,
	UP,
	DOWN
};

/// SaveState of GodAttackComponent, including a saveState of all projectiles, a saveState of the currentAttack and of all attacks' cooldown
USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FAttacksSaveState {
	GENERATED_BODY()

	/// id of the current attack
	int idCurrentAttack;
	/// saveState of the current attack
	UAttackSaveState attackState;
	/// Frame of the current attack montage
	float montageFrame;
	/// Array of all coolDowns
	TArray<int> coolDowns;
	/// Array of saveState of all projectiles
	TMap<int, FSimplifiedProjectileArray> projectiles;

	FAttacksSaveState() : idCurrentAttack(-1) {}
};

/// Struct containing Projectiles pointers in array
USTRUCT(BlueprintType)
struct FProjectileArray {
	GENERATED_BODY()

	UPROPERTY(Category = "Attack", VisibleAnywhere)
	TArray<AHitBoxGroupProjectile*> Projectiles;
};

/**
* Component managing all attacks
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYTHBUSTERS_API UGodAttackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/// Map of all projectiles stored in arrays, one array by attacks (FProjectileArray is used because unreal doesn't supporter containers containing other containers)
	UPROPERTY(Category = "Attack", VisibleAnywhere)
	TMap<int, FProjectileArray> AllProjectiles;

public:	
	// Sets default values for this component's properties
	UGodAttackComponent();

	/// Current Attack
	UPROPERTY(Category = "Attack", VisibleAnywhere, BlueprintReadWrite)
		int currentAttack = -1;
	/// Array of all intanciated attacks
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		TArray<UAttack*> Attacks;
	
	/// Class of the attack Normal + Neutral
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Neutral;
	/// Class of the attack Normal + Forward
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Forward;
	/// Class of the attack Normal + Backward
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Backward;
	/// Class of the attack Normal + Up
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Up;
	/// Class of the attack Normal + Down
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Down;

	/// Class of the attack Special + Neutral
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Neutral;
	/// Class of the attack Special + Forward
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Forward;
	/// Class of the attack Special + Backward
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Backward;
	/// Class of the attack Special + Up
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Up;
	/// Class of the attack Special + Down
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Down;

	/// Class of the attack Normal + Forward
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Forward;
	/// Class of the attack Normal + Backward
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Backward;
	/// Class of the attack Normal + Up
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Up;
	/// Class of the attack Normal + Down
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Down;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	/** Tries to apply a simplified array from a save state to the projectiles of a sequential attack
	* Only succeed if the is exactly the right amount of projetciles, from the same subAttacks, in the same order
	* return if the operation succeeded
	*/
	bool TryApplySequentialProjectileArray(int idAtt, FAttacksSaveState &saveState);
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// Starts a normal attack based on the orientation. Returns true if the attack has successfully beeen sarted
	UFUNCTION(BlueprintCallable)
	bool StartNormalAttack(EAttackDirection _attackDirection);
	/// Starts a special attack based on the orientation. Returns true if the attack has successfully beeen sarted
	UFUNCTION(BlueprintCallable)
	bool StartSpecialAttack(EAttackDirection _attackDirection);
	/// Starts a push attack based on the orientation. Returns true if the attack has successfully beeen sarted
	UFUNCTION(BlueprintCallable)
	bool StartPushAttack(EAttackDirection _attackDirection);
	/// Interrupts the current attack (useful for ejection for example)
	UFUNCTION(BlueprintCallable)
	void InterruptAttack();

	/// Returns if an attack is active
	bool GetIsCurrentAttack() const;
	
	/// Transmits notify to the current attack
	void TransmitNotify(ENotifyType _notifyType);

	/// Registers a newly spawned projectiles to AllProjectiles 
	void RegisterProjectile(AHitBoxGroupProjectile* _projectile, int _idAttack);
	/// Cleans AllProjectiles from invalide pointers (dead projectiles)
	void CleanUpProjectile();

	/// Performs a SaveState of the component
	UFUNCTION(BlueprintCallable)
	FAttacksSaveState SaveAttacksState();
	/// Loads the given save state
	UFUNCTION(BlueprintCallable)
	void LoadAttacksState(FAttacksSaveState saveState);
};
