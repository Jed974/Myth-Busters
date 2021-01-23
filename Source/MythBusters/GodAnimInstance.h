// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "god.h"
#include "HitBoxGroup.h"
#include "Engine/World.h"
#include "GodAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class MYTHBUSTERS_API UGodAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	/** Reference to the pawn owner of type God */
	UPROPERTY(Category = "References", EditAnywhere, BlueprintReadWrite)
		AGod* God;

	/** Copies the State of the owner God */
	UPROPERTY(Category = "References", EditAnywhere, BlueprintReadWrite)
		EGodState GodState;


	/** Value to blend the fly animation on X axis */
	UPROPERTY(Category = "BlendSpace Values", EditAnywhere, BlueprintReadWrite)
		float SpeedX;
	/** Value to blend the fly animation on Y axis */
	UPROPERTY(Category = "BlendSpace Values", EditAnywhere, BlueprintReadWrite)
		float SpeedY;
	/** Value to blend the Eject animation on X axis */
	UPROPERTY(Category = "BlendSpace Values", EditAnywhere, BlueprintReadWrite)
		float EjectX;
	/** Value to blend the Eject animation on Y axis */
	UPROPERTY(Category = "BlendSpace Values", EditAnywhere, BlueprintReadWrite)
		float EjectY;



	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* ShieldAnimation;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TurnMontage;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* MoveBlendSpace;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* DashBlendSpace;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* EjectBlendSpace;
	
public :
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UFUNCTION(BlueprintCallable)
	void HandleAttackNotify(ENotifyType notifyType);

	UFUNCTION(BlueprintCallable)	
	virtual void InterruptAllMontages();
};