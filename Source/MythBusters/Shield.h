// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AHitBoxGroup;
class AGod;
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class MYTHBUSTERS_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();


protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ShieldStaticMesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*> ShieldHitBoxes;
	UPROPERTY(Category = "Material", EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* ShieldMaterialOriginal;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInstanceDynamic* ShieldMaterialInstance = nullptr;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly)
		float finalFillRate = 0.1f;	// same value for every shield

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
		float lifeTime = 0;
	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
		FVector2D InputDirection = FVector2D::ZeroVector;

	float DELTA_TIME;
	AGod* godOwner;


public:
	UPROPERTY(Category = "Shield", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float decreaseDuration = 3;


protected:
	virtual void BeginPlay() override;

	/// Update the rotation of the shield based on InputDirection
	virtual void SetAngleFromVector();


public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitShield(float _size, float _lifeTime, FColor _colorFresnel, FColor _colorBase);

	
	void SetLifeTime(float _lifeTime);
	void SetAngle(float _angle);
	void SetMaterialFillRate(float _fillRate);
	void SetInputDirectionVectorX(float _inputX);
	void SetInputDirectionVectorY(float _inputY);

	float GetLifeTime();

	//UFUNCTION(BlueprintCallable)
	void ProtectGodFromDamage(AHitBoxGroup* hitBoxGroup);
};
