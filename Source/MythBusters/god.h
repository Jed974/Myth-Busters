// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GodMovementComponent.h"
#include "GameFramework/Actor.h"
#include "god.generated.h"

UCLASS()
class MYTHBUSTERS_API Agod : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Agod();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;

protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Movement", meta = (AllowPrivateAccess = "true"))
		UGodMovementComponent* GodMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	        virtual void MoveHorizontal(float AxisValue);
	UFUNCTION(BlueprintImplementableEvent)
	        void EMoveHorizontal(float AxisValue);

	UFUNCTION(BlueprintCallable)
	        virtual void MoveVertical(float AxisValue);
	UFUNCTION(BlueprintImplementableEvent)
	        void EMoveVertical(float AxisValue);

	UFUNCTION(BlueprintCallable)
		virtual void AttackNormal();
	UFUNCTION(BlueprintImplementableEvent)
	        void EAttackNormal();

	UFUNCTION(BlueprintCallable)
		virtual void StopAttackNormal();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopAttackNormal();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

};
