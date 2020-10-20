// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "god.h"
#include "HitBoxGroup.h"
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


	/** Variable to set to true to get in the Hurt State */
	UPROPERTY(Category = "Transition Booleans", EditAnywhere, BlueprintReadWrite)
		bool Hurted;
	/** Variable to set to true to get in the Eject State */
	UPROPERTY(Category = "Transition Booleans", EditAnywhere, BlueprintReadWrite)
		bool Ejected;
	/** Variable to set to true to get in the Shield State */
	UPROPERTY(Category = "Transition Booleans", EditAnywhere, BlueprintReadWrite)
		bool Block;
	/** Variable to set to true to get in the Dash State */
	UPROPERTY(Category = "Transition Booleans", EditAnywhere, BlueprintReadWrite)
		bool Dashing;
	

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
	/** Variable to set to true to get in a PushAtt SubState */
	UPROPERTY(Category = "Attack : Properties", EditAnywhere, BlueprintReadWrite)
		bool AttCanStop;

	
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

public :
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
