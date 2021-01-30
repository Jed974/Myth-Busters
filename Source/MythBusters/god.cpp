// Fill out your copyright notice in the Description page of Project Settings.


#include "god.h"
#include "HitBoxGroup.h"
#include "HitBox.h"
#include "GodAnimInstance.h"
#include <string>
#include <MythBusters\MythBustersGameInstance.h>


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

	GodMovement->ChangeMovementStateDelegate.BindUObject(this, &AGod::UpdateState);
	GodMovement->InstantTurnDelegate.BindUObject(this, &AGod::InstantTurn);

	GodShield = CreateDefaultSubobject<UGodShieldComponent>("GodShieldComponent");

	GodAttack = CreateDefaultSubobject<UGodAttackComponent>("GodAttackComponent");

	GodBoost = CreateDefaultSubobject<UGodBoostComponent>("GodBoostComponent");
}

// Called when the game starts or when spawned
void AGod::BeginPlay()
{
	Super::BeginPlay();
	canMove = true;
	
}

// Called every frame
void AGod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (netplay)
	{
		ReadInputs(&GGPOInputs);
	}
	else
	{
		ReadInputs(&Inputs);
	}
			
	//Check les collisions :
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, TSubclassOf<AHitBoxGroup>());
	for (auto& hitBoxGroupActor : OverlappingActors) {
		AHitBoxGroup* hitBoxGroup = Cast<AHitBoxGroup>(hitBoxGroupActor);
		if (hitBoxGroup != nullptr)
			HandleHitBoxGroupCollision(hitBoxGroup);
	}
	TurnTimer++;
	UpdateAttackState();
}


void AGod::MoveHorizontal(float AxisValue)
{
	if (State == EGodState::Shielding) {
		GodShield->OrientShieldX(AxisValue);
	}
	else
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
	
}
void AGod::MoveVertical(float AxisValue)
{
	if (State == EGodState::Shielding) {
		GodShield->OrientShieldY(AxisValue);
	}
	else
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
}


void AGod::AttackNormal()
{
	if (canAttNorm) {
		switch (State)
		{
		case EGodState::Flying:
			if (GodAttack->StartNormalAttack(attackState)) {
				if (attackState == EAttackDirection::BACKWARD)
				{
					GodMovement->isFacingRight = !GodMovement->isFacingRight;
				}
				EAttackNormal();
				ChangeGodState(EGodState::Attacking);
			}
			break;
		case EGodState::FlyingTurnaround:
			if (GodAttack->StartNormalAttack(attackState)) {
				EAttackNormal();
				ChangeGodState(EGodState::Attacking);
			}
			break;
		}
	}	
}
void AGod::StopAttackNormal()
{
	EStopAttackNormal();
}
void AGod::AttackSpecial()
{
	if (canAttSpe) {
		switch (State)
		{
		case EGodState::Flying:
			if (GodAttack->StartSpecialAttack(attackState)) {
				if (attackState == EAttackDirection::BACKWARD)
				{
					GodMovement->isFacingRight = !GodMovement->isFacingRight;
				}
				EAttackSpecial();
				ChangeGodState(EGodState::Attacking);
			}
			break;
		case EGodState::FlyingTurnaround:
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Backward attack");
			if (GodAttack->StartSpecialAttack(attackState)) {
				EAttackSpecial();
				ChangeGodState(EGodState::Attacking);
			}
			break;
		}
	}
}
void AGod::StopAttackSpecial()
{
	EStopAttackSpecial();
}
void AGod::AttackPush()
{
	if (canAttPush) {
		switch (State)
		{
		case EGodState::Flying:
			if (GodAttack->StartPushAttack(attackState)) {
				if (attackState == EAttackDirection::BACKWARD)
				{
					GodMovement->isFacingRight = !GodMovement->isFacingRight;
				}
				EAttackPush();
				ChangeGodState(EGodState::Attacking);
			}
			break;
		case EGodState::FlyingTurnaround:
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Backward attack");
			if (GodAttack->StartPushAttack(attackState)) {
				EAttackPush();
				ChangeGodState(EGodState::Attacking);
			}
			break;
		}
	}
}
void AGod::StopAttackPush()
{
	EStopAttackPush();
}

void AGod::Shield()	
{
	if (canShield && State==EGodState::Flying && GodShield->StartShield()) {
		ChangeGodState(EGodState::Shielding);
		EShield();
	}
}
void AGod::StopShield()
{
	if (State == EGodState::Shielding) {
		ChangeGodState(EGodState::Flying);
		EStopShield();
	}
}


void AGod::Eject(FVector2D _EjectionSpeed)
{
	ChangeGodState(EGodState::Ejected);
	GodMovement->Eject(_EjectionSpeed);
}


void AGod::Dash()
{
	if (canDash) {
		switch (State)
		{
		case EGodState::Flying:
			ChangeGodState(EGodState::Dashing);
			GodMovement->Dash();
			break;
		}
	}	
}
void AGod::StopDash()
{
	switch (State)
	{
	case EGodState::Dashing:
		ChangeGodState(EGodState::Flying);
		break;
	}
}

void AGod::ChangeGodState(EGodState NewState)
{
	/*if (State == EGodState::Shielding && NewState != EGodState::Shielding)
		StopShield();*/

	// Security anti-attack-lock
	if (State == EGodState::Attacking && GodAttack->GetIsCurrentAttack()) {
		GodAttack->InterruptAttack();
	}
	// Détruit le bouclier
	else if (State == EGodState::Shielding && NewState != EGodState::Shielding) {
		GodShield->StopShield();
	}

	State = NewState;
	canMove = true;
	switch (State)
	{
		case EGodState::Flying:
			break;
		case EGodState::FlyingTurnaround:
			TurnaroundEvent();
			TurnTimer = 0;
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
			canMove = false;
			GodMovement->ChangeMovementState(EMovementState::Attacking);
			break;
		case EGodState::Shielding:
			canMove = false;			
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

	/*PlayerInputComponent->BindAxis("MoveHorizontal", this, &AGod::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &AGod::MoveVertical);
	PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &AGod::AttackNormal);
	PlayerInputComponent->BindAction("AttackNormal", IE_Released, this, &AGod::StopAttackNormal);

	PlayerInputComponent->BindAction("AttackSpecial", IE_Pressed, this, &AGod::AttackSpecial);
	PlayerInputComponent->BindAction("AttackSpecial", IE_Released, this, &AGod::StopAttackSpecial);

	PlayerInputComponent->BindAction("AttackPush", IE_Pressed, this, &AGod::AttackPush);
	PlayerInputComponent->BindAction("AttackPush", IE_Released, this, &AGod::StopAttackPush);

	PlayerInputComponent->BindAction("Shield", IE_Pressed, this, &AGod::Shield);
	PlayerInputComponent->BindAction("Shield", IE_Released, this, &AGod::StopShield);

	PlayerInputComponent->BindAction("Dash",IE_Pressed,this, &AGod::Dash);*/

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &AGod::WriteHorizontalAxis);
	PlayerInputComponent->BindAxis("MoveVertical", this, &AGod::WriteVerticalAxis);
	PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &AGod::PressAttackNormal);
	PlayerInputComponent->BindAction("AttackNormal", IE_Released, this, &AGod::ReleaseAttackNormal);

	PlayerInputComponent->BindAction("AttackSpecial", IE_Pressed, this, &AGod::PressAttackSpecial);
	PlayerInputComponent->BindAction("AttackSpecial", IE_Released, this, &AGod::ReleaseAttackSpecial);

	PlayerInputComponent->BindAction("AttackPush", IE_Pressed, this, &AGod::PressAttackPush);
	PlayerInputComponent->BindAction("AttackPush", IE_Released, this, &AGod::ReleaseAttackPush);

	PlayerInputComponent->BindAction("Shield", IE_Pressed, this, &AGod::PressShield);
	PlayerInputComponent->BindAction("Shield", IE_Released, this, &AGod::ReleaseShield);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGod::PressDash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AGod::ReleaseDash);
}

#pragma region WriteRegion
void AGod::WriteInputs(EInputSpecifier Specifier, float Value)
{
	Inputs.Update(Specifier, Value);
}
void AGod::WriteInputs(EInputSpecifier Specifier, EInputActionState ActionState)
{
	Inputs.Update(Specifier, ActionState);
}
void AGod::WriteVerticalAxis(float Value)
{
	WriteInputs(VERTICAL, Value);
}
void AGod::WriteHorizontalAxis(float Value)
{
	WriteInputs(HORIZONTAL, Value);
}

void AGod::PressAttackNormal()
{
	WriteInputs(NORMAL, Pressed);
}
void AGod::ReleaseAttackNormal()
{
	WriteInputs(NORMAL, Released);
}
void AGod::PressAttackSpecial()
{
	WriteInputs(SPECIAL, Pressed);
}
void AGod::ReleaseAttackSpecial()
{
	WriteInputs(SPECIAL, Released);
}
void AGod::PressAttackPush()
{
	WriteInputs(PUSH, Pressed);
}
void AGod::ReleaseAttackPush()
{
	WriteInputs(PUSH, Released);
}
void AGod::PressShield()
{
	WriteInputs(SHIELD, Pressed);
}
void AGod::ReleaseShield()
{
	WriteInputs(SHIELD, Released);
}
void AGod::PressDash()
{
	WriteInputs(DASH, Pressed);
}
void AGod::ReleaseDash()
{
	WriteInputs(DASH, Released);
}
#pragma endregion

void AGod::ReadInputs(SInputs* _Inputs)
{
	/*SAbstractGameState gs = UMythBustersGameInstance::Instance->gs;
	NonGameState ngs = UMythBustersGameInstance::Instance->ngs;*/
	
	/*FILE* fp = nullptr;
	fopen_s(&fp, "ReadInputsLog.txt", "a");
	if (fp)
	{
		fprintf(fp, "  Frame %i - Player %i : %f\n", UMythBustersGameInstance::Instance->gs._framenumber, GetController()->GetUniqueID() , GGPOInputs.VerticalAxis.Value);
		fclose(fp);
	}*/
	MoveHorizontal(_Inputs->HorizontalAxis.Value);
	MoveVertical(_Inputs->VerticalAxis.Value);
	if (!_Inputs->InputActions[NORMAL].Consumed)
	{
		if (_Inputs->InputActions[NORMAL].State == Pressed)
		{
			AttackNormal();
			_Inputs->InputActions[NORMAL].Consumed = true;
		}
	}
	else
	{
		if (_Inputs->InputActions[NORMAL].State == Released)
		{
			StopAttackNormal();
			_Inputs->InputActions[NORMAL].Consumed = false;
		}
	}
	if (!_Inputs->InputActions[SPECIAL].Consumed)
	{
		if (_Inputs->InputActions[SPECIAL].State == Pressed)
		{
			AttackSpecial();
			_Inputs->InputActions[SPECIAL].Consumed = true;
		}
	}
	else
	{
		if (_Inputs->InputActions[SPECIAL].State == Released)
		{
			StopAttackSpecial();
			_Inputs->InputActions[SPECIAL].Consumed = false;
		}
	}
	if (!_Inputs->InputActions[PUSH].Consumed)
	{
		if (_Inputs->InputActions[PUSH].State == Pressed)
		{
			AttackPush();
			_Inputs->InputActions[PUSH].Consumed = true;
		}
	}
	else
	{
		if (_Inputs->InputActions[PUSH].State == Released)
		{
			StopAttackPush();
			_Inputs->InputActions[PUSH].Consumed = false;
		}
	}
	if (!_Inputs->InputActions[SHIELD].Consumed)
	{
		if (_Inputs->InputActions[SHIELD].State == Pressed)
		{
			Shield();
			_Inputs->InputActions[SHIELD].Consumed = true;
		}
	}
	else
	{
		if (_Inputs->InputActions[SHIELD].State == Released)
		{
			StopShield();
			_Inputs->InputActions[SHIELD].Consumed = false;
		}
	}
	if (!_Inputs->InputActions[DASH].Consumed)
	{
		if (_Inputs->InputActions[DASH].State == Pressed)
		{
			Dash();
			_Inputs->InputActions[DASH].Consumed = true;
		}
	}
	else
	{
		if (_Inputs->InputActions[DASH].State == Released)
		{
			StopDash();
			_Inputs->InputActions[DASH].Consumed = false;
		}
	}
	
}
void AGod::SetupGodLimitation(bool _canDash, bool _canAttNorm, bool _canAttSpe, bool _canAttPush, bool _canShield) {
	canDash = _canDash;
	canAttNorm = _canAttNorm;
	canAttSpe = _canAttSpe;
	canAttPush = _canAttPush;
	canShield = _canShield;
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
void AGod::SetPlayerColor(int depth) {
	//SkeletalMesh->CustomDepthStencilValue = depth;
	SkeletalMesh->SetCustomDepthStencilValue(depth);
	SkeletalMesh->bRenderCustomDepth = true;
	GodShield->SetNoneCustomColors(depth);
}


USkeletalMeshComponent* AGod::GetSkeletalMesh() {
	return SkeletalMesh;
}

void AGod::ApplyGodDamage(float value) {
	this->GodDamage += value;
}

void AGod::UpdateState(EMovementState NewMovementState)
{
	if (canMove)
	{
		switch (NewMovementState)
		{
		case EMovementState::Flying:
			ChangeGodState(EGodState::Flying);
			break;
		case EMovementState::FlyingTurnaroud:
			ChangeGodState(EGodState::FlyingTurnaround);
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
		case EMovementState::Ejected:
			ChangeGodState(EGodState::Ejected);
			break;
		case EMovementState::DeathEjected:
			ChangeGodState(EGodState::Dead);
			break;
		default:;
		}
	}
}


void AGod::UpdateAttackState() {

	float horizAxis = 0;
	float vertAxis = 0;
	if (netplay) {
		horizAxis = GGPOInputs.HorizontalAxis.Value;
		vertAxis = GGPOInputs.VerticalAxis.Value;
	}
	else {
		horizAxis = Inputs.HorizontalAxis.Value;
		vertAxis = Inputs.VerticalAxis.Value;
	}

	if (FMath::Abs(vertAxis) > FMath::Abs(horizAxis)) {		// l'axe vertical gagne
		if (FMath::Abs(vertAxis) > VerticalDeadZone*2) {
			if (vertAxis > 0) {
				attackState = EAttackDirection::UP;
			}
			else {
				attackState = EAttackDirection::DOWN;
			}
		}
		else {
			attackState = EAttackDirection::NEUTRAL;
		}
	}
	else {
		if (FMath::Abs(horizAxis) > HorizontalDeadZone*2) {
			if (TurnTimer >= BackwardAttackFrameWindow) {
				attackState = EAttackDirection::FORWARD;
			}
			else {
				attackState = EAttackDirection::BACKWARD;
			}
		}
		else {
			attackState = EAttackDirection::NEUTRAL;
		}
	}
}

void AGod::InstantTurn()
{
	FRotator NewRotation = SkeletalMesh->GetRelativeRotation();
	NewRotation.Yaw = GodMovement->GetIsFacingRight() ? -90.0f : 90.0f;
	SkeletalMesh->SetRelativeRotation(NewRotation);
}


void AGod::HandleHitBoxGroupCollision(AHitBoxGroup* hitBoxGroup) {
	// Check if the god hit hasn't already been hit or is instigator
	if (hitBoxGroup->GodHitIsValid(this)) {
		// Get the hitBox colliding with actor having highest priority
		UHitBox* HighestPriorityHB = nullptr;
		auto subclass = TSubclassOf<UHitBox>();
		TArray<UHitBox*> components;
		hitBoxGroup->GetComponents(components);
		//auto components = hitBoxGroup->GetComponentsByClass(subclass);
		for (auto const &HB : components) {
			if (HB != nullptr && HB->IsOverlappingActor(this)) {
				if (HighestPriorityHB == nullptr || HighestPriorityHB->Priority < HB->Priority)
					HighestPriorityHB = HB;
			}
		}

		// Check final que le hitbox selectionné n'est pas nul
		if (HighestPriorityHB != nullptr) {
			// Interrupt attacks
			Cast<UGodAnimInstance>(SkeletalMesh->GetAnimInstance())->InterruptAllMontages();
			GodAttack->InterruptAttack();

			// Perform ejection
			FVector2D ejectionVector(1, 0);
			ejectionVector = ejectionVector.GetRotated(HighestPriorityHB->AngleDeg);
			if (!hitBoxGroup->facingRight)
				ejectionVector.X *= -1;
			ejectionVector *= (GodDamage / 10000 + 1) * HighestPriorityHB->BaseKnockBack;
			Eject(ejectionVector);

			// Add dammages
			ApplyGodDamage(HighestPriorityHB->BaseDamage);

			// Register god as hit
			hitBoxGroup->alreadyHit = true;
			//hitBoxGroup->RegisterGodHit(this);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Unexpected error : HitBoxGroup touched god but hitBox couldn't be found");
		}
	}
}

void AGod::HandleAttackNotify(ENotifyType notifyType) {
	GodAttack->TransmitNotify(notifyType);
	if (notifyType == ENotifyType::OVER)
	{
		ChangeGodState(EGodState::Flying);
		GodMovement->ChangeHorizontalMovementState(EHorizontalMovementState::HorizontalNeutral);
	}
		
}
void AGod::RegisterProjectile(AHitBoxGroupProjectile* _projectile, int _idAttack) {
	GodAttack->RegisterProjectile(_projectile, _idAttack);
}
char SInputs::SendGodSelection(char act) {
	if (UMythBustersGameInstance::Instance->gs._framenumber == 1) {
		if (UMythBustersGameInstance::Instance->SelectedGods[0] == 1) {
			act += 0b00100000;
		}
	}
	return act;
}

void SInputs::ReadGodSelection(char act) {
	if (UMythBustersGameInstance::Instance->SelectedGods[1] == -1) {
		UMythBustersGameInstance::Instance->SelectedGods[1] = ((act & 0b00010000) != 0) ? 1 : 0;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Read %i", ((act & 0b00010000) != 0) ? 1 : 0);
	}
}


