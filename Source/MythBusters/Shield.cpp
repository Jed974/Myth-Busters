// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"

// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ShieldStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	ShieldStaticMesh->SetupAttachment(GetRootComponent());

	for (int i = 0; i < 8; i++) {
		FString _componentName = "Shield_HitBox_";
		_componentName.AppendInt(i);
		ShieldHitBoxes.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(_componentName)));
		ShieldHitBoxes[i]->SetupAttachment(ShieldStaticMesh);
	}
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(0, 0, 90), ETeleportType::None);

	if (ShieldMaterialInstance == nullptr) {
		ShieldMaterialInstance = UMaterialInstanceDynamic::Create(ShieldMaterialOriginal, this);
		ShieldStaticMesh->SetMaterial(0, ShieldMaterialInstance);
	}

	DELTA_TIME = 1.0 / GEngine->FixedFrameRate;
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	SetAngleFromVector();
	Super::Tick(DeltaTime);
	if (lifeTime < decreaseDuration) {
		lifeTime += DELTA_TIME;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::SanitizeFloat(lifeTime));
		float _decrecreaseDurationDivided = decreaseDuration / 8.0;
		
		for (int i = 0; i < 7; i++) {
			if (lifeTime > _decrecreaseDurationDivided * (i + 2)) {	// i+1 : visual is late | i+2 : collision disabling is late => better
				ShieldHitBoxes[7-i]->SetVisibility(false);
				ShieldHitBoxes[7-i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}

		if (lifeTime >= decreaseDuration)
			SetMaterialFillRate(finalFillRate);
		else
			SetMaterialFillRate(1 - (lifeTime / decreaseDuration)*(1-finalFillRate));
	}
}


void AShield::InitShield(float _size, FColor _colorFresnel, FColor _colorBase) {
	SetActorRotation(FRotator(0, 0, 90), ETeleportType::None);
	SetActorScale3D(FVector(_size, _size, _size));


	if (ShieldMaterialInstance == nullptr) {
		ShieldMaterialInstance = UMaterialInstanceDynamic::Create(ShieldMaterialOriginal, this);
		ShieldStaticMesh->SetMaterial(0, ShieldMaterialInstance);
	}

	ShieldMaterialInstance->SetVectorParameterValue("ColorFresnel", FLinearColor::FromSRGBColor(_colorFresnel));
	ShieldMaterialInstance->SetVectorParameterValue("ColorBase", FLinearColor::FromSRGBColor(_colorBase));
	ShieldMaterialInstance->SetScalarParameterValue("PushAmount", 10*_size);

}



void AShield::SetAngle(float _angle) {
	SetActorRotation(FRotator(_angle, 0, 90), ETeleportType::None);
}
void AShield::SetAngleFromVector() {
	InputDirection.Normalize();

	if (InputDirection.GetAbsMax() > 0.5f) {
		//float _aimAtAngle = FMath::RadiansToDegrees(acosf(FMath::Abs(InputDirection.X)));
		float _aimAtAngle = FMath::RadiansToDegrees(acosf(InputDirection.X));

		if (InputDirection.Y <= 0)
			_aimAtAngle = FMath::Abs(_aimAtAngle);
		else
			_aimAtAngle = FMath::Abs(_aimAtAngle) * -1;

		SetAngle(_aimAtAngle);
	}
}

void AShield::SetMaterialFillRate(float _fillRate) {
	ShieldMaterialInstance->SetScalarParameterValue("FillRate", _fillRate);
}


void AShield::SetInputDirectionVectorX(float _inputX) {
	InputDirection.X = _inputX;
}
void AShield::SetInputDirectionVectorY(float _inputY) {
	InputDirection.Y = _inputY;
}