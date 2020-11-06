// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shield.h"
#include "GodMovementComponent.h"
#include "GameFramework/Actor.h"
#include "god.generated.h"


UENUM(BlueprintType)
enum class EGodState : uint8 {
	Flying UMETA(DisplayName = "Flying"),
	FlyingTurnaround UMETA(DisplayName = "FlyingTurnaround"),
	Dashing UMETA(DisplayName = "Dashing"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Ejected UMETA(DisplayName = "Ejected"),
	WallHit,
	Hurt UMETA(DisplayName = "Hurt"),
	Attacking UMETA(DisplayName = "Attacking"),
	Shielding UMETA(DisplayName = "Shielding"),
	Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class MYTHBUSTERS_API AGod : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGod();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		float GodDamage = 0;

protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Movement", meta = (AllowPrivateAccess = "true"))
		UGodMovementComponent* GodMovement;

	float HorizontalDeadZone = 0.15f;
	float VerticalDeadZone = 0.15f;
	
	bool canMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AShield> ShieldClassToSpwan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldFresnelColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldBaseColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		float ShieldSize = 1.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		AShield* CurrentShield = nullptr;

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


	UFUNCTION(BlueprintCallable)
		virtual void AttackSpecial();
	UFUNCTION(BlueprintImplementableEvent)
		void EAttackSpecial();

	UFUNCTION(BlueprintCallable)
		virtual void StopAttackSpecial();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopAttackSpecial();


	UFUNCTION(BlueprintCallable)
		virtual void AttackPush();
	UFUNCTION(BlueprintImplementableEvent)
		void EAttackPush();

	UFUNCTION(BlueprintCallable)
		virtual void StopAttackPush();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopAttackPush();

	   
	UFUNCTION(BlueprintCallable)
		virtual void Shield();
	UFUNCTION(BlueprintImplementableEvent)
		void EShield();

	UFUNCTION(BlueprintCallable)
		virtual void StopShield();
	UFUNCTION(BlueprintImplementableEvent)
		void EStopShield();


	UFUNCTION(BlueprintCallable)
		virtual void Eject(FVector2D _EjectionSpeed);

	UFUNCTION(BlueprintCallable)
	virtual void Dash();

	void Die();

	UFUNCTION(BlueprintCallable)
		virtual void SetGodDamage(float value) { this->GodDamage = value; }
	UFUNCTION(BlueprintCallable)
		virtual float GetGodDamage() { return this->GodDamage; }
	UFUNCTION(BlueprintCallable)
		virtual void ApplyGodDamage(float value);
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

	virtual float GetAnimValues(int _idValueToGet);

	virtual USkeletalMeshComponent* GetSkeletalMesh();

	UFUNCTION(BlueprintCallable)
		virtual void ChangeGodState(EGodState NewState);

	UPROPERTY(BlueprintReadOnly)
	EGodState State;

	
};
