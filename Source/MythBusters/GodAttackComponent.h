// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AHitBoxGroupProjectile;
#include "CoreMinimal.h"
#include "Attack.h"
#include "Components/ActorComponent.h"
#include "GodAttackComponent.generated.h"


UENUM(BlueprintType)
enum class ENotifyType : uint8 {
	OVER,
	ACTIVE,
	CHANGE,
	INACTIVE
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

	int idCurrentAttack;
	UAttackSaveState* attackState;
	//TMap<int, TArray<AHitBoxGroupProjectile>> AllProjectiles;

	FAttacksSaveState() : idCurrentAttack(-1) {
		attackState = nullptr;
	}
};
USTRUCT()
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
	UPROPERTY(Category = "Attack", VisibleAnywhere, BlueprintReadWrite)
	int currentAttack = -1;
	UPROPERTY(Category = "Attack", VisibleAnywhere)
	TMap<int, FProjectileArray> AllProjectiles;

public:	
	// Sets default values for this component's properties
	UGodAttackComponent();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, CATEGORY = "Attack", meta = (AllowPrivateAccess = "true"))
		TArray<UAttack*> Attacks;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Neutral;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Forward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Backward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Up;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Normal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Normal_Down;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Neutral;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Forward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Backward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Up;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Special", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Special_Down;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Forward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Backward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Up;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, CATEGORY = "Attack Push", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAttack> Att_Push_Down;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	bool StartNormalAttack(EAttackDirection _attackDirection);
	UFUNCTION(BlueprintCallable)
	bool StartSpecialAttack(EAttackDirection _attackDirection);
	UFUNCTION(BlueprintCallable)
	bool StartPushAttack(EAttackDirection _attackDirection);
	
	void TransmitNotify(ENotifyType _notifyType);

	void RegisterProjectile(AHitBoxGroupProjectile* _projectile, int _idAttack);
	void CleanUpProjectile();

	UFUNCTION(BlueprintCallable)
		FAttacksSaveState SaveAttacksState();
	UFUNCTION(BlueprintCallable)
		void LoadAttacksState(FAttacksSaveState saveState);
};
