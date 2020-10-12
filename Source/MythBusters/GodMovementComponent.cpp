// Fill out your copyright notice in the Description page of Project Settings.


#include "GodMovementComponent.h"
#include "god.h"



// Sets default values for this component's properties
UGodMovementComponent::UGodMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	isFacingRight = true;
	isFacingUp = true;
}


// Called when the game starts
void UGodMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DELTA_TIME = 1.0 / GEngine->FixedFrameRate;
	// ...
	
}

void UGodMovementComponent::ChangeHorizontalMovementState(EHorizontalMovementState NewState)
{
	CurrentHorizontalStateTimer = 0;
	HorizontalMovementState = NewState;
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

	//FVector NewLocation = ComputeNewLocation();

	FVector Location = GetOwner()->GetActorLocation();
	ComputeNewLocation(Location);
	FHitResult hitInfo = FHitResult();
	GetOwner()->SetActorLocation(Location, true, &hitInfo);
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, hitInfo.ImpactPoint.ToString());
	_MovementInput = FVector2D(0.0, 0.0);
}

void UGodMovementComponent::AddMovementInput(const FVector2D Direction, const float Amount)
{
	_MovementInput += Direction*Amount;
}

void UGodMovementComponent::ComputeNewLocation(FVector & Location)
{
	switch (HorizontalMovementState)
	{
	case HorizontalNeutral:
		HorizontalSpeed = 0;
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
			HorizontalSpeed -= MaxHorizontalFlySpeed / HorizontalFlyStopTime;
			CurrentHorizontalStateTimer++;
			if (isFacingRight)
			{
				Location.X += HorizontalSpeed * DELTA_TIME;
			}
			else
			{
				Location.X += HorizontalSpeed * DELTA_TIME * -1;
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
				Location.X += _MovementInput.X * HorizontalSpeed * DELTA_TIME;
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
			Location.X += _MovementInput.X * HorizontalSpeed * DELTA_TIME;
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
				Location.X += HorizontalSpeed * DELTA_TIME;
			}
			else
			{
				Location.X += HorizontalSpeed * DELTA_TIME * -1;
			}
		}
		else
		{
			HorizontalSpeed = 0;
			ChangeHorizontalMovementState(HorizontalNeutral);
		}
		break;
	}

	switch (VerticalMovementState)
	{
	case VerticalNeutral:
		VerticalSpeed = 0;
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
			VerticalSpeed -= MaxVerticalFlySpeed / VerticalFlyStopTime;
			CurrentVerticalStateTimer++;
			if (isFacingUp)
			{
				Location.Z += VerticalSpeed * DELTA_TIME;
			}
			else
			{
				Location.Z += VerticalSpeed * DELTA_TIME * -1;
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
				Location.Z += _MovementInput.Y * VerticalSpeed * DELTA_TIME;
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
			Location.Z += _MovementInput.Y * VerticalSpeed * DELTA_TIME;
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
				Location.Z += VerticalSpeed * DELTA_TIME;
			}
			else
			{
				Location.Z += VerticalSpeed * DELTA_TIME * -1;
			}
			
		}
		else
		{
			VerticalSpeed = 0;
			ChangeVerticalMovementState(VerticalNeutral);
		}
		break;
	}
}
