// Fill out your copyright notice in the Description page of Project Settings.


#include "GodMovementComponent.h"
#include "god.h"



// Sets default values for this component's properties
UGodMovementComponent::UGodMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	isFacingRight = true;
	isFacingUp = true;
}


// Called when the game starts
void UGodMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DELTA_TIME = 1.0 / GEngine->FixedFrameRate;
	
}

void UGodMovementComponent::ChangeHorizontalMovementState(EHorizontalMovementState NewState)
{
	CurrentHorizontalStateTimer = 0;
	HorizontalMovementState = NewState;
	HorizontalPreviousSpeed = Velocity.X;
	switch (NewState)
	{
		case FlyHorizontalStartup:
			isFacingRight = _MovementInput.X > 0;
			break;
	}
}

void UGodMovementComponent::ChangeVerticalMovementState(EVerticalMovementState NewState)
{
	CurrentVerticalStateTimer = 0;
	VerticalMovementState = NewState;
	VerticalPreviousSpeed = Velocity.Y;
	switch (NewState)
	{
		case FlyVerticalStartup:
			isFacingUp = _MovementInput.Y > 0;
			break;
	}
}


// Called every frame
void UGodMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DELTA_TIME, TickType, ThisTickFunction);

	FVector Location = GetOwner()->GetActorLocation();
	ComputeNewVelocity();
	Location.X += Velocity.X * DELTA_TIME;
	Location.Z += Velocity.Y * DELTA_TIME;
	FHitResult hitInfo = FHitResult();
	GetOwner()->SetActorLocation(Location, true, &hitInfo);
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Velocity.ToString());
	_MovementInput = FVector2D(0.0, 0.0);
}

void UGodMovementComponent::AddMovementInput(const FVector2D Direction, const float Amount)
{
	_MovementInput += Direction*Amount;
}

void UGodMovementComponent::ComputeNewVelocity()
{
	switch (HorizontalMovementState)
	{
	case HorizontalNeutral:
		HorizontalSpeed = 0;
		Velocity.X = HorizontalSpeed;
		if (_MovementInput.X < 0.0 && isFacingRight || _MovementInput.X > 0.0 && !isFacingRight)
		{
			ChangeHorizontalMovementState(HorizontalTurnAround);
		}
		else if (_MovementInput.X > 0.0 && isFacingRight || _MovementInput.X < 0.0 && !isFacingRight)
		{
			ChangeHorizontalMovementState(FlyHorizontalStartup);
		}
		break;
	case FlyHorizontalStop:
		if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyStopTime * DELTA_TIME)
		{
			HorizontalSpeed = FMath::Abs(HorizontalPreviousSpeed) * (1 - CurrentHorizontalStateTimer / HorizontalFlyStopTime);
			CurrentHorizontalStateTimer++;
			if (isFacingRight)
			{
				//Location.X += HorizontalSpeed * DELTA_TIME;
				Velocity.X = HorizontalSpeed;
			}
			else
			{
				//Location.X += HorizontalSpeed * DELTA_TIME * -1;
				Velocity.X = HorizontalSpeed * -1;
			}
		}
		else
		{
			HorizontalSpeed = 0;
			ChangeHorizontalMovementState(HorizontalNeutral);
		}
		break;
	case HorizontalTurnAround:
		if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalTurnaroundTime * DELTA_TIME)
		{
			CurrentHorizontalStateTimer++;
		}
		else
		{
			ChangeHorizontalMovementState(FlyHorizontalStartup);
		}
		break;
	case FlyHorizontalStartup:
		if (_MovementInput.X > 0.0 && isFacingRight || _MovementInput.X < 0.0 && !isFacingRight)
		{
			if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyStartupTime * DELTA_TIME)
			{
				HorizontalSpeed += MaxHorizontalFlySpeed / HorizontalFlyStartupTime;
				CurrentHorizontalStateTimer++;
				//Location.X += _MovementInput.X * HorizontalSpeed * DELTA_TIME;
				Velocity.X = _MovementInput.X * HorizontalSpeed;
			}
			else
			{
				ChangeHorizontalMovementState(FlyHorizontal);
			}		
		}
		else
		{
			ChangeHorizontalMovementState(HorizontalNeutral);
		}

		break;
	case FlyHorizontal:
		if (_MovementInput.X > 0.0 && isFacingRight || _MovementInput.X < 0.0 && !isFacingRight)
		{
			//Location.X += _MovementInput.X * HorizontalSpeed * DELTA_TIME;
			Velocity.X = _MovementInput.X * HorizontalSpeed;
		}
		else if (_MovementInput.X == 0.0)
		{
			ChangeHorizontalMovementState(FlyHorizontalStop);
		}
		else
		{
			ChangeHorizontalMovementState(FlyHorizontalTurnAround);
		}
		break;
	case FlyHorizontalTurnAround:
		if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyTurnaroundTime * DELTA_TIME)
		{
			HorizontalSpeed -= MaxHorizontalFlySpeed / HorizontalFlyTurnaroundTime;
			CurrentHorizontalStateTimer++;
			if (isFacingRight)
			{
				//Location.X += HorizontalSpeed * DELTA_TIME;
				Velocity.X = HorizontalSpeed;
			}
			else
			{
				//Location.X += HorizontalSpeed * DELTA_TIME * -1;
				Velocity.X = HorizontalSpeed * -1;
			}
		}
		else
		{
			HorizontalSpeed = 0;
			ChangeHorizontalMovementState(HorizontalNeutral);
		}
		break;
	case HorizontalEjection:
		if (CurrentHorizontalStateTimer * DELTA_TIME < EjectionRecoverTime * DELTA_TIME * 60)
		{
			float alpha = CurrentHorizontalStateTimer / (EjectionRecoverTime * 60);
			HorizontalSpeed = FMath::Lerp(EjectionVelocity.X, _MovementInput.X * MaxHorizontalFlySpeed, alpha);
			Velocity.X = HorizontalSpeed;
			
			CurrentHorizontalStateTimer++;
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Ejection over");

			EjectionVelocity = FVector2D::ZeroVector;
			
			if (_MovementInput.X != 0)
			{
				isFacingRight = 0 < _MovementInput.X;
				Velocity.X = _MovementInput.X * MaxHorizontalFlySpeed;
				HorizontalSpeed = FMath::Abs(Velocity.X);
				ChangeHorizontalMovementState(FlyHorizontal);
			}/*
			if (_MovementInput.X > 0)
			{
				isFacingRight = true;
				ChangeHorizontalMovementState(FlyHorizontal);
			}
			else if (_MovementInput.X > 0)
			{
				isFacingRight = false;
				ChangeHorizontalMovementState(FlyHorizontal);
			}*/
			else {
				ChangeHorizontalMovementState(HorizontalNeutral);
			}			
		}
		break;
	}

	switch (VerticalMovementState)
	{
	case VerticalNeutral:
		VerticalSpeed = 0;
		Velocity.Y = VerticalSpeed;
		if (_MovementInput.Y < 0.0 && isFacingUp || _MovementInput.Y > 0.0 && !isFacingUp)
		{
			ChangeVerticalMovementState(VerticalTurnAround);
		}
		else if (_MovementInput.Y > 0.0 && isFacingUp || _MovementInput.Y < 0.0 && !isFacingUp)
		{
			ChangeVerticalMovementState(FlyVerticalStartup);
		}
		break;
	case FlyVerticalStop:
		if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStopTime * DELTA_TIME)
		{
			VerticalSpeed = FMath::Abs(VerticalPreviousSpeed) * (1 - CurrentVerticalStateTimer / VerticalFlyStopTime);
			CurrentVerticalStateTimer++;
			if (isFacingUp)
			{
				//Location.Z += VerticalSpeed * DELTA_TIME;
				Velocity.Y = VerticalSpeed;
			}
			else
			{
				//Location.Z += VerticalSpeed * DELTA_TIME * -1;
				Velocity.Y = VerticalSpeed * -1;
			}
		}
		else
		{
			VerticalSpeed = 0;
			ChangeVerticalMovementState(VerticalNeutral);
		}
		break;
	case VerticalTurnAround:
		if (CurrentVerticalStateTimer * DELTA_TIME < VerticalTurnaroundTime * DELTA_TIME)
		{
			CurrentVerticalStateTimer++;
		}
		else
		{
			ChangeVerticalMovementState(FlyVerticalStartup);
		}
		break;
	case FlyVerticalStartup:
		if (_MovementInput.Y > 0.0 && isFacingUp || _MovementInput.Y < 0.0 && !isFacingUp)
		{
			if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStartupTime * DELTA_TIME)
			{
				VerticalSpeed += MaxVerticalFlySpeed / VerticalFlyStartupTime;
				CurrentVerticalStateTimer++;
				//Location.Z += _MovementInput.Y * VerticalSpeed * DELTA_TIME;
				Velocity.Y = _MovementInput.Y * VerticalSpeed;
			}
			else
			{
				ChangeVerticalMovementState(FlyVertical);
			}
		}
		else
		{
			ChangeVerticalMovementState(VerticalNeutral);
		}

		break;
	case FlyVertical:

		if (_MovementInput.Y > 0.0 && isFacingUp || _MovementInput.Y < 0.0 && !isFacingUp)
		{
			//Location.Z += _MovementInput.Y * VerticalSpeed * DELTA_TIME;
			Velocity.Y = _MovementInput.Y * VerticalSpeed;
		}
		else if (_MovementInput.Y == 0.0)
		{
			ChangeVerticalMovementState(FlyVerticalStop);
		}
		else
		{
			ChangeVerticalMovementState(FlyVerticalTurnAround);
		}
		break;
	case FlyVerticalTurnAround:
		if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyTurnaroundTime * DELTA_TIME)
		{
			VerticalSpeed -= MaxVerticalFlySpeed / VerticalFlyTurnaroundTime;
			CurrentVerticalStateTimer++;
			if (isFacingUp)
			{
				//Location.Z += VerticalSpeed * DELTA_TIME;
				Velocity.Y = VerticalSpeed;
			}
			else
			{
				//Location.Z += VerticalSpeed * DELTA_TIME * -1;
				Velocity.Y = VerticalSpeed * -1;
			}

		}
		else
		{
			VerticalSpeed = 0;
			ChangeVerticalMovementState(VerticalNeutral);
		}
		break;
	case VerticalEjection:
		if (CurrentVerticalStateTimer * DELTA_TIME < EjectionRecoverTime * DELTA_TIME * 60)
		{
			float alpha = CurrentVerticalStateTimer / (EjectionRecoverTime * 60);
			VerticalSpeed = FMath::Lerp(EjectionVelocity.Y, _MovementInput.Y * MaxVerticalFlySpeed, alpha);
			
			Velocity.Y = VerticalSpeed;
			CurrentVerticalStateTimer++;
		}
		else
		{		
			EjectionVelocity = FVector2D::ZeroVector;

			if (_MovementInput.Y != 0)
			{
				isFacingUp = _MovementInput.Y > 0;
				Velocity.Y = _MovementInput.Y * MaxVerticalFlySpeed;
				VerticalSpeed = FMath::Abs(Velocity.Y);
				ChangeVerticalMovementState(FlyVertical);
			}
			else {
				ChangeVerticalMovementState(VerticalNeutral);
			}
		}
		break;
	}
}

void UGodMovementComponent::EjectGod(FVector2D _EjectionSpeed) {
	EjectionVelocity.X = _EjectionSpeed.X;
	EjectionVelocity.Y = _EjectionSpeed.Y;
	isFacingRight = EjectionVelocity.X < 0;
	isFacingUp = EjectionVelocity.Y < 0;

	ChangeHorizontalMovementState(HorizontalEjection);
	ChangeVerticalMovementState(VerticalEjection);
}
