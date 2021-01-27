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


UENUM(BlueprintType)
enum class ENotifyType : uint8 {
	OVER,
	ACTIVE,
	CHANGE,
	INACTIVE,
	NEXT
};
UENUM(BlueprintType)
enum class EAttackDirection : uint8 {
	NEUTRAL,
	FORWARD,
	BACKWARD,
	UP,
	DOWN
};

USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FAttacksSaveState {
	GENERATED_BODY()

	int idCurrentSubAttack;
	UAttackSaveState attackState;
	float montageFrame;
	TArray<int> coolDowns;
	TMap<int, FSimplifiedProjectileArray> projectiles;

	FAttacksSaveState() : idCurrentSubAttack(-1) {}
};

USTRUCT(BlueprintType)
struct FProjectileArray {
	GENERATED_BODY()

	UPROPERTY(Category = "Attack", VisibleAnywhere)
	TArray<AHitBoxGroupProjectile*> Projectiles;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYTHBUSTERS_API UGodAttackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(Category = "Attack", VisibleAnywhere)
	TMap<int, FProjectileArray> AllProjectiles;

public:	
	// Sets default values for this component's properties
	UGodAttackComponent();


	UPROPERTY(Category = "Attack", VisibleAnywhere, BlueprintReadWrite)
		int currentAttack = -1;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		TArray<UAttack*> Attacks;
	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Neutral;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Forward;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Backward;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Up;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Down;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Neutral;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Forward;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Backward;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Up;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Down;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Forward;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Backward;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Up;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Down;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool TryApplySequentialProjectileArray(int idAtt, FAttacksSaveState &saveState);
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	bool StartNormalAttack(EAttackDirection _attackDirection);
	UFUNCTION(BlueprintCallable)
	bool StartSpecialAttack(EAttackDirection _attackDirection);
	UFUNCTION(BlueprintCallable)
	bool StartPushAttack(EAttackDirection _attackDirection);
	UFUNCTION(BlueprintCallable)
	void InterruptAttack();

	bool GetIsCurrentAttack() const;
	
	void TransmitNotify(ENotifyType _notifyType);

	void RegisterProjectile(AHitBoxGroupProjectile* _projectile, int _idAttack);
	void CleanUpProjectile();

	UFUNCTION(BlueprintCallable)
	FAttacksSaveState SaveAttacksState();
	UFUNCTION(BlueprintCallable)
	void LoadAttacksState(FAttacksSaveState saveState);
};
