// Fill out your copyright notice in the Description page of Project Settings.


#include "god.h"

// Sets default values
Agod::Agod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(GetRootComponent());
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(CapsuleComponent);
	
	GodMovement = CreateDefaultSubobject<UGodMovementComponent>("GodMovementComponent");

}

// Called when the game starts or when spawned
void Agod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Agod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void Agod::MoveHorizontal(float AxisValue)
{
	GodMovement->AddMovementInput(FVector2D(1.0, 0.0), AxisValue);
}

void Agod::MoveVertical(float AxisValue)
{
	GodMovement->AddMovementInput(FVector2D(0.0, 1.0), AxisValue);
}


void Agod::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &Agod::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &Agod::MoveVertical);
}


