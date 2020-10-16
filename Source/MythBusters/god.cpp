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
	EMoveHorizontal(AxisValue);
}

void Agod::MoveVertical(float AxisValue)
{
	GodMovement->AddMovementInput(FVector2D(0.0, 1.0), AxisValue);
	EMoveVertical(AxisValue);
}

<<<<<<< HEAD
void Agod::AttackNormal()
{
	EAttackNormal();
}

void Agod::StopAttackNormal()
{
	EStopAttackNormal();
}
=======
void Agod::InputDash()
{
	GodMovement->Dash();
}



>>>>>>> e53ba9f993cc4f74714d3b923f5d7c7c15f2817b

void Agod::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &Agod::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &Agod::MoveVertical);
<<<<<<< HEAD
	PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &Agod::AttackNormal);
	PlayerInputComponent->BindAction("AttackNormal", IE_Released, this, &Agod::StopAttackNormal);

=======
	PlayerInputComponent->BindAction("Dash",IE_Pressed,this, &Agod:: InputDash);
>>>>>>> e53ba9f993cc4f74714d3b923f5d7c7c15f2817b
}


