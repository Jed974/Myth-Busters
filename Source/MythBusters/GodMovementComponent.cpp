// Fill out your copyright notice in the Description page of Project Settings.


#include "GodMovementComponent.h"
#include "god.h"



// Sets default values for this component's properties
UGodMovementComponent::UGodMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//OwnerGod = dynamic_cast<AGod*>(GetOwner());
}


// Called when the game starts
void UGodMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DELTA_TIME = 1.0 / GEngine->FixedFrameRate;
	isFacingRight = true;
	isFacingUp = true;
	IsPushable = true;
	
}

void UGodMovementComponent::ChangeHorizontalMovementState(EHorizontalMovementState NewState)
{
	CurrentHorizontalStateTimer = 0;
	HorizontalMovementState = NewState;
	HorizontalPreviousSpeed = Velocity.X;
	switch (NewState)
	{
		case FlyHorizontalStartup:
			ChangeMovementState(EMovementState::Flying);
			break;
		case FlyHorizontalTurnAround:
			ChangeMovementState(EMovementState::FlyingTurnaroud);
			break;
		case HorizontalTurnAround:
			ChangeMovementState(EMovementState::FlyingTurnaroud);
			break;
		case HorizontalNeutral:
			ChangeMovementState(EMovementState::Flying);
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
			break;
	}
}

void UGodMovementComponent::ChangeMovementState(EMovementState NewState)
{
	ChangeMovementStateDelegate.ExecuteIfBound(NewState);
	MovementState = NewState;
	HorizontalPreviousSpeed = Velocity.X;
	VerticalPreviousSpeed = Velocity.Y;
	switch (MovementState)
	{
		case EMovementState::WallHit:
			WallHitFrameCounter = 0;
			break;
		case EMovementState::Dashing:
			DashFrameCounter = 0;
			DashStartupCounter = 0;
			DashLagCounter = 0;
			break;
		case EMovementState::FlyingTurnaroud:
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Turnaround");
			break;
	}
}


// Called every frame
void UGodMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Location = GetOwner()->GetActorLocation();
	FHitResult HitInfo = FHitResult();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, isFacingRight ? "Right" : "Left" ); 
	if (CollidingActor != nullptr && IsPushable && CollidingActor->Tags.Contains(FName("Pusher")))
	{		
		ComputePushVelocity(CollidingActor);
		Location.X += PushVelocity.X * DELTA_TIME;
		Location.Z += PushVelocity.Y * DELTA_TIME;
	}
	
	if (MovementState == EMovementState::DeathEjected)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Velocity.ToString());
		Location.X += Velocity.X * DELTA_TIME;
		Location.Z += Velocity.Y * DELTA_TIME;
		GetOwner()->SetActorLocation(Location, false, &HitInfo, ETeleportType::TeleportPhysics);
	}
	else
	{
		ComputeNewVelocity();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Velocity.ToString());
		Location.X += Velocity.X * DELTA_TIME;
		Location.Z += Velocity.Y * DELTA_TIME;
		
		GetOwner()->SetActorLocation(Location, true, &HitInfo);
		if (HitInfo.GetActor() != nullptr)
		{
			Location = GetOwner()->GetActorLocation();

			ComputeWallMovement(HitInfo);
			Location.X += Velocity.X * DELTA_TIME;
			Location.Z += Velocity.Y * DELTA_TIME;
			GetOwner()->SetActorLocation(Location, true);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, HitInfo.Normal.ToString());
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Velocity.ToString());
		}

		if (HorizontalMovementState == SprintHorizontal || HorizontalMovementState == SprintHorizontalStartup || HorizontalMovementState == SprintHorizontalStop || VerticalMovementState == SprintVerticalStartup || VerticalMovementState == SprintVerticalStop)
		{
			ChangeMovementState(EMovementState::Sprinting);
		}
	}

	
	_MovementInput = FVector2D(0.f, 0.f);
}

void UGodMovementComponent::AddMovementInput(const FVector2D Direction, const float Amount)
{
	_MovementInput += Direction * FMath::Clamp(Amount, -1.f, 1.f);
}

void UGodMovementComponent::Dash()
{
	ChangeMovementState(EMovementState::Dashing);
}

void UGodMovementComponent::Eject(FVector2D _EjectionSpeed) {
	EjectionVelocity = _EjectionSpeed;
	isFacingRight = EjectionVelocity.X < 0;
	isFacingUp = EjectionVelocity.Y < 0;
	ChangeMovementState(EMovementState::Ejected);
}

void UGodMovementComponent::ComputeNewVelocity() {
	switch (MovementState)
	{
		case EMovementState::Flying:
			ComputeFlyingVelocity();
			break;
		case EMovementState::FlyingTurnaroud:
			ComputeFlyingVelocity();
			break;
		case EMovementState::Dashing:
			ComputeDashingVelocity();
			break;
		case EMovementState::Ejected:
			ComputeEjectedVelocity();
			break;
		case EMovementState::WallHit:
			if (WallHitFrameCounter < WallHitFrames)
			{
				WallHitFrameCounter++;
			}
			else
			{
				Eject(EjectionVelocity);
			}
			break;
		//case EMovementState::DeathEjected:
			
			//break;

	}
	

}

void UGodMovementComponent::ComputeWallMovement(FHitResult HitInfo)
{
	FVector Tangent = FVector(0.f, 0.f, 0.f);
	FVector2D Reflect = FVector2D::ZeroVector;
	AArena* _arenaHitted;
	switch (MovementState)
	{
		case EMovementState::Flying:
			if (_MovementInput.Y != 0.f)
			{
				Tangent = HitInfo.Normal.RotateAngleAxis((HitInfo.Normal.X > 0.f && HitInfo.Normal.Z < 0.f || HitInfo.Normal.X < 0.f && HitInfo.Normal.Z > 0.f ? -89.f : 89.f), FVector(0.f, 1.f, 0.f)) * MaxVerticalFlySpeed;
			}
			else if (_MovementInput.X != 0.f)
			{
				Tangent = HitInfo.Normal.RotateAngleAxis((HitInfo.Normal.X < 0.f && HitInfo.Normal.Z < 0.f || HitInfo.Normal.X > 0.f && HitInfo.Normal.Z > 0.f ? -89.f : 89.f), FVector(0.f, 1.f, 0.f)) * MaxHorizontalFlySpeed;
			}
			Velocity.X = Tangent.X;
			Velocity.Y = Tangent.Z;
			break;
		case EMovementState::Ejected:
			_arenaHitted = Cast<AArena>(HitInfo.Actor);
			if (_arenaHitted != nullptr) {
				bool _godDie = _arenaHitted->IsEjected(GetVelocityNorm(), HitInfo);
				if (_godDie) {
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "A player died");
			
					Velocity = EjectionVelocity;
					Velocity.Normalize();
					Velocity = Velocity * 2000;

					ChangeMovementState(EMovementState::DeathEjected);
					
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "A player bounced");
					EjectionVelocity = EjectionVelocity - 2 * (FVector2D::DotProduct(EjectionVelocity, FVector2D(HitInfo.Normal.X, HitInfo.Normal.Z))) * FVector2D(HitInfo.Normal.X, HitInfo.Normal.Z);
					Velocity = FVector2D::ZeroVector;
					ChangeMovementState(EMovementState::WallHit);
				}
			}
			
			
			break;
		case EMovementState::Dashing:
			Reflect = Velocity - 2 * (FVector2D::DotProduct(Velocity, FVector2D(HitInfo.Normal.X, HitInfo.Normal.Z))) * FVector2D(HitInfo.Normal.X, HitInfo.Normal.Z);
			if (isFacingRight != Reflect.X > 0)
			{
				isFacingRight = Reflect.X > 0;
				InstantTurnDelegate.ExecuteIfBound();
			}
			
			isFacingUp = Reflect.Y > 0;
			Reflect.Normalize();
			DashDir = Reflect;
			DashFrameCounter -= DashFrameCounter*0.75f;

			break;
		case EMovementState::DeathEjected:
			break;
	}
	
}

void UGodMovementComponent::ComputeDashingVelocity()
{

	if (DashStartupCounter < DashStartupFrames)
	{
		if (FMath::Abs(_MovementInput.X) > 0.1)
		{
			DashDir.X = _MovementInput.X;
		}
		else
		{
			DashDir.X = isFacingRight ? 1 : -1;
		}
		
		if (FMath::Abs(_MovementInput.Y) > 0.1)
		{
			DashDir.Y = _MovementInput.Y;
			if (FMath::Abs(_MovementInput.X) < 0.1)
			{
				DashDir.X = 0;
			}
		}
		else
		{
			DashDir.Y = 0;
		}
		DashDir.Normalize();
		DashStartupCounter++;
	}
	else if (DashFrameCounter < DashFrames)
	{
		const float Alpha = float(DashFrameCounter) / DashFrames;
		Velocity.X = FMath::Lerp(DashingSpeed * DashDir.X, 0.0f, DashSpeedProfile.EditorCurveData.Eval(Alpha));
		Velocity.Y = FMath::Lerp(DashingSpeed * DashDir.Y, 0.0f, DashSpeedProfile.EditorCurveData.Eval(Alpha));
		DashFrameCounter++;
	}
	else
	{
		if (DashLagCounter < DashLagFrames)
		{
			DashLagCounter++;
		}
		else
		{
			ChangeHorizontalMovementState(HorizontalNeutral);
			ChangeVerticalMovementState(VerticalNeutral);
		}
		
		/*if (_MovementInput.X != 0)
		{
			ChangeHorizontalMovementState(SprintHorizontal);
		}
		else
		{
			ChangeHorizontalMovementState(HorizontalNeutral);
		}
		if (_MovementInput.Y != 0)
		{
			ChangeVerticalMovementState(SprintVertical);
		}
		else
		{
			ChangeVerticalMovementState(VerticalNeutral);
		}*/
		
		
	}
}

void UGodMovementComponent::ComputeEjectedVelocity()
{
	if (EjectionFrameCounter * DELTA_TIME < EjectionRecoverTime * DELTA_TIME * 60)
	{
		const float Alpha = EjectionFrameCounter / (EjectionRecoverTime * 60);
		HorizontalSpeed = FMath::Lerp(EjectionVelocity.X, _MovementInput.X * MaxHorizontalFlySpeed, Alpha);
		VerticalSpeed = FMath::Lerp(EjectionVelocity.Y, _MovementInput.Y * MaxVerticalFlySpeed, Alpha);
		Velocity.X = HorizontalSpeed;
		Velocity.Y = VerticalSpeed;

		EjectionFrameCounter++;
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Ejection over");
		EjectionFrameCounter = 0;
		//EjectionVelocity = FVector2D::ZeroVector;

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
		ChangeMovementState(EMovementState::Flying);
	}
	
}



void UGodMovementComponent::ComputeFlyingVelocity()
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
		if (CurrentHorizontalStateTimer < HorizontalFlyStopTime)
		{
			if (_MovementInput.X != 0.0f)
			{
				if (CurrentHorizontalStateTimer >= HorizontalFlyStopCancelFrames)
				{
					HorizontalSpeed = 0;
					ChangeHorizontalMovementState(HorizontalNeutral);
					break;
				}
				else
				{
					if (FMath::Sign(_MovementInput.X) != (isFacingRight ? 1.0f : -1.0f))
					{
						ChangeHorizontalMovementState(FlyHorizontalTurnAround);
						break;
					}
					
				}
				
			}
			HorizontalSpeed = FMath::Abs(HorizontalPreviousSpeed) * (1.0f - static_cast<float>(CurrentHorizontalStateTimer) / HorizontalFlyStopTime);
			if (HorizontalSpeed == 0.0f)
			{
				CurrentHorizontalStateTimer = HorizontalFlyStopTime;
			}
			else
			{
				CurrentHorizontalStateTimer++;
			}
			Velocity.X = HorizontalSpeed * (isFacingRight ? 1.0f : -1.0f);
			if (_MovementInput.X != 0.0f && FMath::Sign(_MovementInput.X) != (isFacingRight ? 1.0f : - 1.0f))
			{
				ChangeHorizontalMovementState(FlyHorizontalTurnAround);
			}
		}
		else
		{
			HorizontalSpeed = 0;
			ChangeHorizontalMovementState(HorizontalNeutral);
		}
		break;
	case HorizontalTurnAround:
		if (CurrentHorizontalStateTimer < HorizontalTurnaroundTime)
		{
			CurrentHorizontalStateTimer++;
		}
		else
		{
			ChangeHorizontalMovementState(FlyHorizontalStartup);
			isFacingRight = !isFacingRight;
		}
		break;
	case FlyHorizontalStartup:
		if (_MovementInput.X > 0.0 && isFacingRight || _MovementInput.X < 0.0 && !isFacingRight)
		{
			if (CurrentHorizontalStateTimer  < HorizontalFlyStartupTime)
			{
				const float Alpha = FMath::Pow(float(CurrentHorizontalStateTimer) / (HorizontalFlyStartupTime-1), 2);
				HorizontalSpeed = FMath::Lerp(StartupHorizontalFlySpeed, MaxHorizontalFlySpeed, Alpha);
				CurrentHorizontalStateTimer++;
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
			Velocity.X = _MovementInput.X * HorizontalSpeed;
		}
		else if (_MovementInput.X == 0.0)
		{
			ChangeHorizontalMovementState(FlyHorizontalStop);
		}
		else
		{
			if (FMath::Abs(_MovementInput.X) > 0.50f)
			{
				ChangeHorizontalMovementState(FlyHorizontalTurnAround);
			}
			else
			{
				ChangeHorizontalMovementState(HorizontalTurnAround);
			}
			
		}
		break;
	case FlyHorizontalTurnAround:
		if (CurrentHorizontalStateTimer < HorizontalFlyTurnaroundTime)
		{
			HorizontalSpeed -= FMath::Abs(HorizontalPreviousSpeed) / HorizontalFlyTurnaroundTime;
			if (HorizontalSpeed == 0.0f)
			{
				CurrentHorizontalStateTimer = HorizontalFlyTurnaroundTime;
			}
			else
			{
				CurrentHorizontalStateTimer++;
			}
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
			isFacingRight = !isFacingRight;
		}
		break;
	case SprintHorizontal:
		Velocity.X = (isFacingRight ? 1 : -1) * HorizontalSpeed * SprintSpeedScale;
		if (_MovementInput.X == 0.0)
		{
			ChangeHorizontalMovementState(FlyHorizontalStop);
		}
		else if ((_MovementInput.X > 0.0 && !isFacingRight || _MovementInput.X < 0.0 && isFacingRight))
		{
			ChangeHorizontalMovementState(FlyHorizontalTurnAround);
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
		if (CurrentVerticalStateTimer < VerticalFlyStopTime)
		{
			if (_MovementInput.Y != 0.0f)
			{
				if (CurrentVerticalStateTimer >= VerticalFlyStopCancelFrames)
				{
					VerticalSpeed = 0;
					ChangeVerticalMovementState(VerticalNeutral);
					break;
				}
				else
				{
					if (FMath::Sign(_MovementInput.Y) != (isFacingUp ? 1.0f : -1.0f))
					{
						ChangeVerticalMovementState(FlyVerticalTurnAround);
						break;
					}

				}
			}
			VerticalSpeed = FMath::Abs(VerticalPreviousSpeed) * (1.0f - static_cast<float>(CurrentVerticalStateTimer) / VerticalFlyStopTime);

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
		if (CurrentVerticalStateTimer < VerticalTurnaroundTime)
		{
			CurrentVerticalStateTimer++;
		}
		else
		{
			ChangeVerticalMovementState(FlyVerticalStartup);
			isFacingUp = !isFacingUp;
		}
		break;
	case FlyVerticalStartup:
		if (_MovementInput.Y > 0.0 && isFacingUp || _MovementInput.Y < 0.0 && !isFacingUp)
		{
			if (CurrentVerticalStateTimer * DELTA_TIME < VerticalFlyStartupTime * DELTA_TIME)
			{
				const float Alpha = FMath::Pow(float(CurrentVerticalStateTimer) / (VerticalFlyStartupTime - 1), 2);
				VerticalSpeed = FMath::Lerp(StartupVerticalFlySpeed, MaxVerticalFlySpeed, Alpha);
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
			if (FMath::Abs(_MovementInput.Y) > 0.50f)
			{
				ChangeVerticalMovementState(FlyVerticalTurnAround);
			}
			else
			{
				ChangeVerticalMovementState(VerticalTurnAround);
			}
		}
		break;
	case FlyVerticalTurnAround:
		if (CurrentVerticalStateTimer < VerticalFlyTurnaroundTime)
		{
			VerticalSpeed -= FMath::Abs(VerticalPreviousSpeed) / VerticalFlyTurnaroundTime;
			if (VerticalSpeed == 0.0f)
			{
				CurrentVerticalStateTimer = VerticalFlyTurnaroundTime;
			}
			else
			{
				CurrentVerticalStateTimer++;
			}
			
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
			isFacingUp = !isFacingUp;
		}
		break;
	case SprintVertical:
		if (_MovementInput.Y > 0.0 && isFacingUp || _MovementInput.Y < 0.0 && !isFacingUp)
		{
			Velocity.Y = (isFacingUp * 2 - 1) * VerticalSpeed * SprintSpeedScale;
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
	}
}

void UGodMovementComponent::ComputePushVelocity(const AActor* OtherActor)
{
	const float DistanceX = GetOwner()->GetActorLocation().X - OtherActor->GetActorLocation().X;
	const float DistanceY = GetOwner()->GetActorLocation().Z - OtherActor->GetActorLocation().Z;
	const float PushSpeedX = FMath::Sign(DistanceX) * FMath::Clamp(1.0f / FMath::Abs(DistanceX) , 0.23f, 1.0f) * 2 * MaxHorizontalFlySpeed;
	const float PushSpeedY = FMath::Sign(DistanceY) * FMath::Clamp(1.0f / FMath::Abs(DistanceY), 0.23f, 1.0f) * 2 * MaxVerticalFlySpeed;
	FVector PushDirection = GetOwner()->GetActorLocation() - OtherActor->GetActorLocation();
	PushDirection.Normalize();

	if (_MovementInput.X != 0 || _MovementInput.Y != 0)
	{
		PushVelocity.X = FMath::Abs(FMath::Sign(_MovementInput.X)) * PushSpeedX;
		PushVelocity.Y = FMath::Abs(FMath::Sign(_MovementInput.Y)) * PushSpeedY;
	}
	else
	{
		PushVelocity.X = PushSpeedX;
		//PushVelocity.Y = PushSpeedY;
	}
	
	
	
	
}


float UGodMovementComponent::GetVelocityNorm() {
	return FMath::Sqrt(FMath::Square(Velocity.X) + FMath::Square(Velocity.Y));
}
bool UGodMovementComponent::GetIsFacingRight() {
	return isFacingRight;
}