// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "god.h"
#include "HitBoxGroup.h"
#include "Engine/World.h"
#include "GodAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EAttackDir : uint8{
	Neutral,
	Forward,
	Backward,
	UpWard,
	DownWard
};

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
	
	/** Variable to set to true to get in the Dash State */
	UPROPERTY(Category = "Transition Booleans", EditAnywhere, BlueprintReadWrite)
		bool Dashing;
	/** Variable to set to true to stop an animation after it is finished */
	UPROPERTY(Category = "Transition Booleans", EditAnywhere, BlueprintReadWrite)
		bool AnimCanStop;
	

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


	/** Variable to set to true to get in a NormalAtt SubState */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		bool AttNormal;
	/** Variable to set to true to get in a SpecialAtt SubState */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		bool AttSpe;
	/** Variable to set to true to get in a PushAtt SubState */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		bool AttPush;
	/** Variable to set to true to get in a PushAtt SubState */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		EAttackDir AttDirection;
	/** Name of the socket where to attach the Normal Forward Attack hitboxes */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		FName NormalForwardSocketName;
	/** The HitBoxGroup Class to spawn for the Normal Forward Attack */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroup> NormalForwardHitBoxGroupClass;
	/** Name of the socket where to attach the Push Forward Attack hitboxes */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		FName PushForwardSocketName;
	/** The HitBoxGroup Class to spawn for the Push Forward Attack */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroup> PushForwardHitBoxGroupClass;
	/** Name of the socket where to attach the Special Forward Attack hitboxes */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		FName SpecialForwardSocketName;
	/** The HitBoxGroup Class to spawn for the Special Forward Attack*/
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHitBoxGroup> SpecialForwardHitBoxGroupClass;
	
	/** Reference to the pawn owner of type God */
	UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
		AHitBoxGroup* InterruptableAttGroup;
	//
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* NormalNeutralGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* NormalForwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* NormalBackwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* NormalUpwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* NormalDownwardGroup;

	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* SpeNeutralGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* SpeForwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* SpeBackwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* SpeUpwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* SpeDownwardGroup;

	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* PushNeutralGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* PushForwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* PushBackwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* PushUpwardGroup;
	///** Reference to the pawn owner of type God */
	//UPROPERTY(Category = "Attack : HitBoxGroups", EditAnywhere, BlueprintReadWrite)
	//	AHitBoxGroup* PushDownwardGroup;
	//
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* NormalForwardMontage;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* SpecialForwardMontage;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* PushForwardMontage;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* HurtAnimation;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* HurtMontage;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* ShieldAnimation;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TurnMontage;
	UPROPERTY(Category = "Animations", EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TurnReverseMontage;
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
	virtual void InterruptAttack();
	UFUNCTION(BlueprintCallable)
	virtual void StartAttackInterruptable(TSubclassOf<AHitBoxGroup> ClassToSpwan, FName SocketToAttachTo);
	UFUNCTION(BlueprintCallable)
	virtual void StartAttackProjectile(TSubclassOf<AHitBoxGroup> ClassToSpwan, FName SocketToAttachTo);
};