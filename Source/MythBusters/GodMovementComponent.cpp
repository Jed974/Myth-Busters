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
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Location = GetOwner()->GetActorLocation();
	ComputeNewVelocity();
	Location.X += Velocity.X * DELTA_TIME;
	Location.Z += Velocity.Y * DELTA_TIME;
	FHitResult HitInfo = FHitResult();
	GetOwner()->SetActorLocation(Location, true, &HitInfo);
	if (HitInfo.GetActor() != nullptr)
	{
		Location = GetOwner()->GetActorLocation();
		FVector Tangent = FVector(0.f, 0.f, 0.f);
		if (_MovementInput.Y != 0.f)
		{
			Tangent = HitInfo.Normal.RotateAngleAxis((HitInfo.Normal.X > 0.f && HitInfo.Normal.Z < 0.f || HitInfo.Normal.X < 0.f && HitInfo.Normal.Z > 0.f ? -89.f : 89.f), FVector(0.f, 1.f, 0.f)) * MaxVerticalFlySpeed;
		}
		else if (_MovementInput.X != 0.f)
		{
			Tangent = HitInfo.Normal.RotateAngleAxis((HitInfo.Normal.X < 0.f && HitInfo.Normal.Z < 0.f || HitInfo.Normal.X > 0.f && HitInfo.Normal.Z > 0.f ? -89.f : 89.f), FVector(0.f, 1.f, 0.f)) * MaxHorizontalFlySpeed;
		}
		
		Location.X += Tangent.X * DELTA_TIME;
		Location.Z += Tangent.Z * DELTA_TIME;
		GetOwner()->SetActorLocation(Location, true);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, HitInfo.Normal.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Velocity.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Tangent.ToString());
	}
	
	_MovementInput = FVector2D(0.f, 0.f);
}

void UGodMovementComponent::AddMovementInput(const FVector2D Direction, const float Amount)
{
	_MovementInput += Direction * FMath::Clamp(Amount, -1.f, 1.f);
}

void UGodMovementComponent::Dash() {
	HorizontalDashFrameCounter = 0;
	UGodMovementComponent::ChangeHorizontalMovementState(EHorizontalMovementState::DashHorizontal);
	VerticalDashFrameCounter = 0;
	UGodMovementComponent::ChangeVerticalMovementState(EVerticalMovementState::DashVertical);
}

void UGodMovementComponent::Eject(FVector2D _EjectionSpeed) {
	EjectionVelocity.X = _EjectionSpeed.X;
	EjectionVelocity.Y = _EjectionSpeed.Y;
	isFacingRight = EjectionVelocity.X < 0;
	isFacingUp = EjectionVelocity.Y < 0;

	ChangeHorizontalMovementState(EjectionHorizontal);
	ChangeVerticalMovementState(EjectionVertical);
}

void UGodMovementComponent::ComputeNewVelocity() {
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
			HorizontalSpeed = FMath::Abs(HorizontalPreviousSpeed) * (1 - static_cast<float>(CurrentHorizontalStateTimer) / HorizontalFlyStopTime);
			CurrentHorizontalStateTimer++;
			if (isFacingRight)
			{
				Velocity.X = HorizontalSpeed;
			}
			else
			{
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
	case EjectionHorizontal:
		if (CurrentHorizontalStateTimer * DELTA_TIME < EjectionRecoverTime * DELTA_TIME * 60)
		{
			const float Alpha = CurrentHorizontalStateTimer / (EjectionRecoverTime * 60);
			HorizontalSpeed = FMath::Lerp(EjectionVelocity.X, _MovementInput.X * MaxHorizontalFlySpeed, Alpha);
			Velocity.X = HorizontalSpeed;

			CurrentHorizontalStateTimer++;
		}
		else
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Ejection over");*/

			EjectionVelocity = FVector2D::ZeroVector;

			if (_MovementInput.X != 0)
			{
				isFacingRight = _MovementInput.X > 0;
				Velocity.X = _MovementInput.X * MaxHorizontalFlySpeed;
				HorizontalSpeed = FMath::Abs(Velocity.X);
				ChangeHorizontalMovementState(FlyHorizontal);
			}
			else
			{
				ChangeHorizontalMovementState(HorizontalNeutral);
			}
	case DashHorizontal:
		if (HorizontalDashFrameCounter <= DashFrames && (_MovementInput.X > 0.0 && isFacingRight || _MovementInput.X < 0.0 && !isFacingRight))
		{
			Velocity.X = _MovementInput.X * HorizontalSpeed * DashingSpeedScale;
			HorizontalDashFrameCounter++;
		}
		else
		{
			ChangeHorizontalMovementState(FlyHorizontal);
		}
		break;
		}
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
			VerticalSpeed = FMath::Abs(VerticalPreviousSpeed) * (1 - static_cast<float>(CurrentVerticalStateTimer) / VerticalFlyStopTime);
			CurrentVerticalStateTimer++;
			if (isFacingUp)
			{
				Velocity.Y = VerticalSpeed;
			}
			else
			{
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
				Velocity.Y = VerticalSpeed;
			}
			else
			{
				Velocity.Y = VerticalSpeed * -1;
			}

		}
		else
		{
			VerticalSpeed = 0;
			ChangeVerticalMovementState(VerticalNeutral);
		}
		break;
	case EjectionVertical:
		if (CurrentVerticalStateTimer* DELTA_TIME < EjectionRecoverTime * DELTA_TIME * 60)
		{
			const float Alpha = CurrentVerticalStateTimer / (EjectionRecoverTime * 60);
			VerticalSpeed = FMath::Lerp(EjectionVelocity.Y, _MovementInput.Y * MaxVerticalFlySpeed, Alpha);
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
			else 
			{
				ChangeVerticalMovementState(VerticalNeutral);
			}
	case DashVertical:
		if (VerticalDashFrameCounter <= DashFrames && _MovementInput.Y != 0.0)
		{
			Velocity.Y = _MovementInput.Y * VerticalSpeed * DashingSpeedScale;
			VerticalDashFrameCounter++;
		}
		else
		{
			ChangeVerticalMovementState(FlyVertical);
		}
		break;
		}
	}

}