// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "god.h"
#include "HitBoxGroup.h"
#include "Engine/World.h"
#include <Runtime\Engine\Classes\Sound\SoundCue.h>
#include "GodAnimInstance.generated.h"


/**
 * Anim Intances parent class for god's anim Intance. A God's skeletal mesh should always use a GodAnimInstance derivated class.
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

	/// Sound to play when the god is hit by another god
	UPROPERTY(Category = "Sound", EditAnywhere, BlueprintReadWrite)
		USoundCue* hurt_cue;

	/// The AnimSequence to play in loop when the god is shielding
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* ShieldAnimation;
	/// The AnimMontage to play when the god is performing a uturn ; The anim notify "EndTurn" must be at the end + blendIn, blendOut have to be set to 0
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TurnMontage;
	/// The BlendSpace2D to play in loop when the god is moving without dashing
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* MoveBlendSpace;
	/// The BlendSpace2D to play in loop when the god is dashing
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* DashBlendSpace;
	/// The BlendSpace2D to play in loop when the god is ejected
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* EjectBlendSpace;
	
public :
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/// Called to transmits the AnimNotify received to the god (which will transmits them to the attacks) ; if connected in blueprint
	UFUNCTION(BlueprintCallable)
	void HandleAttackNotify(ENotifyType notifyType);

	/// Stops current played montage
	UFUNCTION(BlueprintCallable)	
	virtual void InterruptAllMontages();
};