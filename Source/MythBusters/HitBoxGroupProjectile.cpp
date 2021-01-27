// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBoxGroupProjectile.h"

/*FSimplifiedProjectile::FSimplifiedProjectile() : facingRight(true), lifeTime(0), transform() {}
FSimplifiedProjectile::FSimplifiedProjectile(AHitBoxGroupProjectile* model) : facingRight(model->facingRight),
																				lifeTime(model->LifeTime),
																				transform(model->GetActorTransform()) {}
*/

// Called when the game starts or when spawned
void AHitBoxGroupProjectile::BeginPlay()
{
	Super::BeginPlay();
	Speed = GetActorRotation().RotateVector(Speed);
}

// Called every frame
void AHitBoxGroupProjectile::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	FVector newLocation(GetActorLocation().X + Speed.X, 0, GetActorLocation().Z + Speed.Z);
	SetActorLocation(newLocation);

	LifeTime--;
	if (LifeTime < 0) {
		Destroy();
	}
}

/*FSimplifiedProjectile AHitBoxGroupProjectile::getSimplifiedVersion() {
	return FSimplifiedProjectile(this);
}*/
FSimplifiedProjectile AHitBoxGroupProjectile::getSimplifiedVersion() {
	return FSimplifiedProjectile(this);
}


/*void AHitBoxGroupProjectile::applySimplifiedVersion(const FSimplifiedProjectile& simple) {
	facingRight = simple.facingRight;
	LifeTime = simple.lifeTime;
	SetActorTransform(simple.transform);
}*/
void AHitBoxGroupProjectile::applySimplifiedVersion(const FSimplifiedProjectile& simple) {
	facingRight = simple.facingRight;
	alreadyHit = simple.alreadyHit;
	LifeTime = simple.lifeTime;
	SetActorTransform(simple.transform);
}