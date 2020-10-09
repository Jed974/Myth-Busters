// Fill out your copyright notice in the Description page of Project Settings.


#include "GodMovementComponent.h"



// Sets default values for this component's properties
UGodMovementComponent::UGodMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
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
}

void UGodMovementComponent::ChangeVerticalMovementState(EVerticalMovementState NewState)
{
	CurrentVerticalStateTimer = 0;
	VerticalMovementState = NewState;
}


// Called every frame
void UGodMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector location = GetOwner()->GetActorLocation();
	switch (HorizontalMovementState)
	{
		case HorizontalNeutral:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Idle"));
			HorizontalSpeed = 0;
			if (_MovementInput.X < 0.0)
			{
				ChangeHorizontalMovementState(FlyLeftStartup);
			}
			else if (_MovementInput.X > 0.0)
			{
				ChangeHorizontalMovementState(FlyRightStartup);
			}
			break;
		case FlyLeftStop:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stopping"));
			if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyStopTime * DELTA_TIME)
			{
				HorizontalSpeed -= MaxHorizontalFlySpeed / HorizontalFlyStopTime;
				CurrentHorizontalStateTimer++;
				location.X +=  HorizontalSpeed * DeltaTime * -1;
			}
			else
			{
				HorizontalSpeed = 0;
				ChangeHorizontalMovementState(HorizontalNeutral);
			}
			break;
		case FlyLeftStartup:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Startint Left"));
			if (_MovementInput.X < 0)
			{
				if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyStartupTime * DELTA_TIME)
				{
					HorizontalSpeed += MaxHorizontalFlySpeed / HorizontalFlyStartupTime;
					CurrentHorizontalStateTimer++;
					location.X += _MovementInput.X * HorizontalSpeed * DeltaTime;
				}
				else
				{
					ChangeHorizontalMovementState(FlyLeft);
				}
			}
			else
			{
				ChangeHorizontalMovementState(HorizontalNeutral);
			}
			
			break;
		case FlyLeft:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Left"));
			if (_MovementInput.X < 0.0)
			{
				location.X += _MovementInput.X * HorizontalSpeed * DeltaTime;
			}
			else if (_MovementInput.X == 0.0)
			{
				ChangeHorizontalMovementState(FlyLeftStop);
			}
			else
			{
				ChangeHorizontalMovementState(FlyRight);
			}
			break;
		case FlyRightStop:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stopping"));
			if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyStopTime * DELTA_TIME)
			{
				HorizontalSpeed -= MaxHorizontalFlySpeed / HorizontalFlyStopTime;
				CurrentHorizontalStateTimer++;
				location.X += HorizontalSpeed * DeltaTime;
			}
			else
			{
				HorizontalSpeed = 0;
				ChangeHorizontalMovementState(HorizontalNeutral);
			}
			break;
		case FlyRightStartup:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Starting Right"));
			if (_MovementInput.X > 0)
			{
				if (CurrentHorizontalStateTimer * DELTA_TIME < HorizontalFlyStartupTime * DELTA_TIME)
				{
					HorizontalSpeed += MaxHorizontalFlySpeed / HorizontalFlyStartupTime;
					CurrentHorizontalStateTimer++;
					location.X += _MovementInput.X * HorizontalSpeed * DeltaTime;
				}
				else
				{
					ChangeHorizontalMovementState(FlyRight);
				}
			}
			else
			{
				ChangeHorizontalMovementState(HorizontalNeutral);
			}
			break;
		case FlyRight:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Right"));
			if (_MovementInput.X > 0.0)
			{
				location.X += _MovementInput.X * HorizontalSpeed * DeltaTime;
			}
			else if (_MovementInput.X == 0.0)
			{
				ChangeHorizontalMovementState(FlyRightStop);
			}
			else
			{
				ChangeHorizontalMovementState(FlyLeft);
			}
			break;

	}
	
	switch (VerticalMovementState)
	{
	case VerticalNeutral:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Idle"));
		VerticalSpeed = 0;
		if (_MovementInput.Y < 0.0)
		{
			ChangeVerticalMovementState(FlyDownStartup);
		}
		else if (_MovementInput.Y > 0.0)
		{
			ChangeVerticalMovementState(FlyUpStartup);
		}
		break;
	case FlyDownStop:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stopping"));
		if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStopTime * DELTA_TIME)
		{
			VerticalSpeed -= MaxVerticalFlySpeed / VerticalFlyStopTime;
			CurrentVerticalStateTimer++;
			location.Z += VerticalSpeed * DeltaTime * -1;
		}
		else
		{
			VerticalSpeed = 0;
			ChangeVerticalMovementState(VerticalNeutral);
		}
		break;
	case FlyDownStartup:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Startint down"));
		if (_MovementInput.Y < 0)
		{
			if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStartupTime * DELTA_TIME)
			{
				VerticalSpeed += MaxVerticalFlySpeed / VerticalFlyStartupTime;
				CurrentVerticalStateTimer++;
				location.Z += _MovementInput.Y * VerticalSpeed * DeltaTime;
			}
			else
			{
				ChangeVerticalMovementState(FlyDown);
			}
		}
		else
		{
			ChangeVerticalMovementState(VerticalNeutral);
		}

		break;
	case FlyDown:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Down"));
		if (_MovementInput.Y < 0.0)
		{
			location.Z += _MovementInput.Y * VerticalSpeed * DeltaTime;
		}
		else if (_MovementInput.Y == 0.0)
		{
			ChangeVerticalMovementState(FlyDownStop);
		}
		else
		{
			ChangeVerticalMovementState(FlyUp);
		}
		break;
	case FlyUpStop:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stopping"));
		if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStopTime * DELTA_TIME)
		{
			VerticalSpeed -= MaxVerticalFlySpeed / VerticalFlyStopTime;
			CurrentVerticalStateTimer++;
			location.Z += VerticalSpeed * DeltaTime;
		}
		else
		{
			VerticalSpeed = 0;
			ChangeVerticalMovementState(VerticalNeutral);
		}
		break;
	case FlyUpStartup:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Startint down"));
		if (_MovementInput.Y > 0)
		{
			if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStartupTime * DELTA_TIME)
			{
				VerticalSpeed += MaxVerticalFlySpeed / VerticalFlyStartupTime;
				CurrentVerticalStateTimer++;
				location.Z += _MovementInput.Y * VerticalSpeed * DeltaTime;
			}
			else
			{
				ChangeVerticalMovementState(FlyUp);
			}
		}
		else
		{
			ChangeVerticalMovementState(VerticalNeutral);
		}

		break;
	case FlyUp:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Down"));
		if (_MovementInput.Y > 0.0)
		{
			location.Z += _MovementInput.Y * VerticalSpeed * DeltaTime;
		}
		else if (_MovementInput.Y == 0.0)
		{
			ChangeVerticalMovementState(FlyUpStop);
		}
		else
		{
			ChangeVerticalMovementState(FlyDown);
		}
		break;

	}
	GetOwner()->SetActorLocation(location);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, _MovementInput.ToString());
	_MovementInput = FVector2D(0.0, 0.0);
}

void UGodMovementComponent::AddMovementInput(const FVector2D Direction, const float Amount)
{
	_MovementInput += Direction*Amount;
}
