// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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


public:

	UPROPERTY(Category = "Shield", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float decreaseDuration = 3;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Update the rotation of the shield based on InputDirection
	virtual void SetAngleFromVector();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	virtual void InitShield(float _size, FColor _colorFresnel, FColor _colorBase);

	UFUNCTION(BlueprintCallable)
	virtual void SetAngle(float _angle);
	UFUNCTION(BlueprintCallable)
	virtual void SetMaterialFillRate(float _fillRate);
	UFUNCTION(BlueprintCallable)
	virtual void SetInputDirectionVectorX(float _inputX);
	UFUNCTION(BlueprintCallable)
	virtual void SetInputDirectionVectorY(float _inputY);

};
