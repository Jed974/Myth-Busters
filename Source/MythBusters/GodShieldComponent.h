// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shield.h"
#include "Components/ActorComponent.h"
#include "GodShieldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHBUSTERS_API UGodShieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodShieldComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AShield> ShieldClassToSpwan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldFresnelColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		FColor ShieldBaseColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CATEGORY = "Shield", meta = (AllowPrivateAccess = "true"))
		float ShieldSize = 1.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, CATEGORY = "Shield")
		AShield* CurrentShield = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
