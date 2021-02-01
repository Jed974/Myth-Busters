// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AHitBoxGroup;
class AGod;
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

/** 
* The shield acts like an extension of the hurt box that doesn't take damage or ejection.
* It's initialy a sphere around the player but the angle covered decrease with time or damages blocked
* It's orientable
* 
* Only use the blueprint derivated class already existing in the project
*/
UCLASS()
class MYTHBUSTERS_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();


protected:
	/// Mesh of the shield
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ShieldStaticMesh;
	/// The different Boxes that block damages
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*> ShieldHitBoxes;
	/// The shield material instance parent
	UPROPERTY(Category = "Material", EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* ShieldMaterialOriginal;
	/// The shield material instance (used to change its parameters)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInstanceDynamic* ShieldMaterialInstance = nullptr;

	/// indicates how much (which angle) the shield should be visible (material instance parameters)
	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly)
		float finalFillRate = 0.1f;	// same value for every shield

	/// Lifetime (=energy) of the shield. Begins at 0 (360° cover) and gradually increases
	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
		float lifeTime = 0;
	/// Direction of inputs used to orient the shield
	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
		FVector2D InputDirection = FVector2D::ZeroVector;

	/// Constant indicating a frame duration
	float DELTA_TIME;
	/// Owner of the shield
	AGod* godOwner;


public:
	/// Time (in sec) needed to go from full cover to minimal cover (without being attacked)
	UPROPERTY(Category = "Shield", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float decreaseDuration = 3;


protected:
	virtual void BeginPlay() override;

	/// Update the rotation of the shield based on InputDirection
	virtual void SetAngleFromVector();


public:	
	virtual void Tick(float DeltaTime) override;

	/// Initializes the shield properties
	UFUNCTION(BlueprintCallable)
	void InitShield(float _size, float _lifeTime, FColor _colorFresnel, FColor _colorBase);

	/// Set the shield's lifeTime
	void SetLifeTime(float _lifeTime);
	/// Set the shield's angle
	void SetAngle(float _angle);
	/// Set the shield's material fill rate
	void SetMaterialFillRate(float _fillRate);
	/// Set the shield's InputDirection X value
	void SetInputDirectionVectorX(float _inputX);
	/// Set the shield's InputDirection Y value
	void SetInputDirectionVectorY(float _inputY);

	/// Get the shield's lifeTime
	float GetLifeTime();
	/// Get the shield's angle
	float GetAngle();

	/// Make the shield takes damage instead of its owner and setting the hitboxgroup alreadyHit at true
	void ProtectGodFromDamage(AHitBoxGroup* hitBoxGroup);
};
