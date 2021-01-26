// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class AGod;
#include "Shield.h"
#include "Components/ActorComponent.h"
#include "GodShieldComponent.generated.h"




USTRUCT(BlueprintType)
struct MYTHBUSTERS_API FShieldSaveState {
	GENERATED_BODY()

	bool out;
	int lifeTime;
	float angle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UGodShieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodShieldComponent();

protected:
	AGod* godOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AShield> ShieldClassToSpwan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldFresnelColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldBaseColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		float ShieldSize = 1.8f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Shield")
		AShield* CurrentShield = nullptr;

	float DELTA_TIME;
	float shieldLifeTime = 0;

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool StartShield();
	void StopShield();

	void OrientShieldX(float AxisValue);
	void OrientShieldY(float AxisValue);

	UFUNCTION(BlueprintCallable)
	FShieldSaveState GetShieldSaveState();
	UFUNCTION(BlueprintCallable)
	void ApplyShieldSaveState(FShieldSaveState ShSvSt);
};
