// Fill out your copyright notice in the Description page of Project Settings.


#include "god.h"

// Sets default values
AGod::AGod()
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
void AGod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGod::MoveHorizontal(float AxisValue)
{
	if (FMath::Abs(AxisValue) > HorizontalDeadZone)
	{
		GodMovement->AddMovementInput(FVector2D(1.0, 0.0), AxisValue);
		EMoveHorizontal(AxisValue);
	}
}

void AGod::MoveVertical(float AxisValue)
{
	if (FMath::Abs(AxisValue) > VerticalDeadZone)
	{
		GodMovement->AddMovementInput(FVector2D(0.0, 1.0), AxisValue);
		EMoveVertical(AxisValue);
	}
	
}


void AGod::AttackNormal()
{
	EAttackNormal();
}

void AGod::StopAttackNormal()
{
	EStopAttackNormal();
}

void AGod::Dash()
{
	GodMovement->Dash();
}




void AGod::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &AGod::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &AGod::MoveVertical);
	PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &AGod::AttackNormal);
	PlayerInputComponent->BindAction("AttackNormal", IE_Released, this, &AGod::StopAttackNormal);

	PlayerInputComponent->BindAction("Dash",IE_Pressed,this, &AGod::Dash);
}


float AGod::GetAnimValues(int _idValueToGet) {
	switch (_idValueToGet) {
		case 0 :	// SpeedX
			return FMath::Abs((GodMovement->Velocity.X / GodMovement->MaxHorizontalFlySpeed) * 100);
		case 1:		// SpeedY
			return (GodMovement->Velocity.Y / GodMovement->MaxVerticalFlySpeed) * 100;
		case 2:		// Eject X
			return FMath::Abs((GodMovement->EjectionVelocity.X / FMath::Sqrt(FMath::Square(GodMovement->EjectionVelocity.X) + FMath::Square(GodMovement->EjectionVelocity.Y))));
		case 3:		// Eject Y
			return (GodMovement->EjectionVelocity.Y / FMath::Sqrt(FMath::Square(GodMovement->EjectionVelocity.X) + FMath::Square(GodMovement->EjectionVelocity.Y)));
		default:
			return 0;
	}
}