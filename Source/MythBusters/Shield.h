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
		UMaterialInstanceDynamic* ShieldMaterialInstance;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
		float lifeTime = 0;

	float DELTA_TIME;


public:

	UPROPERTY(Category = "Shield", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float decreaseDuration = 3;
	UPROPERTY(Category = "Shield", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float finalFillRate = 0.1f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetAngle(float _angle);
	UFUNCTION(BlueprintCallable) 
	virtual void SetAngleFromVector(FVector2D _inputDirection);
	UFUNCTION(BlueprintCallable)
	virtual void SetMaterialFillRate(float _fillRate);

};
