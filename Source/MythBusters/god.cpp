// Fill out your copyright notice in the Description page of Project Settings.


#include "god.h"

#include <string>


// Sets default values
AGod::AGod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(GetRootComponent());
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(CapsuleComponent);
	CapsuleComponent->SetCollisionProfileName(FName("God"));
	
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

	switch (GodMovement->MovementState)
	{
		case EMovementState::Flying:
			ChangeGodState(EGodState::Flying);
			break;
		case EMovementState::FlyingTurnaroud:
			ChangeGodState(EGodState::FlyingTurnaround);
			break;
		case EMovementState::Ejected:
			ChangeGodState(EGodState::Ejected);
			break;
		case EMovementState::WallHit:
			ChangeGodState(EGodState::WallHit);
			break;
		case EMovementState::Sprinting:
			ChangeGodState(EGodState::Sprinting);
			break;
		case EMovementState::Dashing: 
			ChangeGodState(EGodState::Dashing);
			break;
		case EMovementState::DeathEjected: 
			ChangeGodState(EGodState::Dead);
			break;
		default: ;
	}
	

}


void AGod::MoveHorizontal(float AxisValue)
{
	if (FMath::Abs(AxisValue) > HorizontalDeadZone)
	{
		GodMovement->AddMovementInput(FVector2D(1.0, 0.0), AxisValue);
		EMoveHorizontal(AxisValue);
	}
	else
	{
		GodMovement->AddMovementInput(FVector2D(1.0, 0.0), 0.f);
		EMoveHorizontal(0.f);
	}
}

void AGod::MoveVertical(float AxisValue)
{
	if (FMath::Abs(AxisValue) > VerticalDeadZone)
	{
		GodMovement->AddMovementInput(FVector2D(0.0, 1.0), AxisValue);
		EMoveVertical(AxisValue);
	}
	else
	{
		GodMovement->AddMovementInput(FVector2D(0.0, 1.0), 0.f);
		EMoveVertical(0.f);
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
void AGod::AttackSpecial()
{
	EAttackSpecial();
}
void AGod::StopAttackSpecial()
{
	EStopAttackSpecial();
}
void AGod::AttackPush()
{
	EAttackPush();
}
void AGod::StopAttackPush()
{
	EStopAttackPush();
}

void AGod::Eject(FVector2D _EjectionSpeed)
{
	ChangeGodState(EGodState::Ejected);
	GodMovement->Eject(_EjectionSpeed);
}


void AGod::Dash()
{
	switch (State)
	{
		case EGodState::Flying:
			ChangeGodState(EGodState::Dashing);
			GodMovement->Dash();
			break;
	}
	
}

void AGod::ChangeGodState(EGodState NewState)
{
	State = NewState;
	switch (State)
	{
	case EGodState::Flying:

		break;
	case EGodState::FlyingTurnaround:

		break;
	case EGodState::Dashing:

		break;
	case EGodState::Sprinting: 
		
		break;
	case EGodState::Ejected: 
		
		break;
	case EGodState::WallHit: 
		
		break;
	case EGodState::Hurt: 
		
		break;
	case EGodState::Attacking: 
		
		break;
	case EGodState::Shielding: 
		
		break;
	case EGodState::Dead: 
		Die();
		break;
	default: ;
	}
}

void AGod::Die()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void AGod::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &AGod::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &AGod::MoveVertical);
	PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &AGod::AttackNormal);
	PlayerInputComponent->BindAction("AttackNormal", IE_Released, this, &AGod::StopAttackNormal);

	PlayerInputComponent->BindAction("AttackSpecial", IE_Pressed, this, &AGod::AttackSpecial);
	PlayerInputComponent->BindAction("AttackSpecial", IE_Released, this, &AGod::StopAttackSpecial);

	PlayerInputComponent->BindAction("AttackPush", IE_Pressed, this, &AGod::AttackPush);
	PlayerInputComponent->BindAction("AttackPush", IE_Released, this, &AGod::StopAttackPush);

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

USkeletalMeshComponent* AGod::GetSkeletalMesh() {
	return SkeletalMesh;
}

void AGod::ApplyDamage(float value) {
	this->GodDamage += value;
}