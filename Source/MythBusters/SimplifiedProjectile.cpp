// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplifiedProjectile.h"
#include "HitBoxGroupProjectile.h"


FSimplifiedProjectile::FSimplifiedProjectile() : facingRight(true), alreadyHit(false), lifeTime(0), transform(), auxiliaryInfo(-1) {}
FSimplifiedProjectile::FSimplifiedProjectile(AHitBoxGroupProjectile* model) : facingRight(model->facingRight), alreadyHit(model->alreadyHit), lifeTime(model->LifeTime), transform(model->GetActorTransform()), auxiliaryInfo(model->auxiliaryInfo) {}
FSimplifiedProjectile::~FSimplifiedProjectile() {}