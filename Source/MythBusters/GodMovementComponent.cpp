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

	// ...
	
}

// Called every frame
void UGodMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector location = GetOwner()->GetActorLocation();
	/*switch (HorizontalMovementState)
	{
		case FlyLeft:
			if (_MovementInput.X <= 0)
			{
				location.X += _MovementInput.X * MaxHorizontalFlySpeed * DeltaTime;
			}
			else
			{
				HorizontalMovementState = FlyTurnAroundLeft;
			}
			break;
		case FlyTurnAroundLeft:

	}*/
	
	location.X += _MovementInput.X * MaxHorizontalFlySpeed * DeltaTime;
	location.Z += _MovementInput.Y * MaxVerticalFlySpeed * DeltaTime;
	GetOwner()->SetActorLocation(location);
	_MovementInput = FVector2D(0.0, 0.0);
}

void UGodMovementComponent::AddMovementInput(const FVector2D Direction, const float Amount)
{
	_MovementInput += Direction*Amount;
}
