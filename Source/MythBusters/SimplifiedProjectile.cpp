// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplifiedProjectile.h"
#include "HitBoxGroupProjectile.h"


FSimplifiedProjectile::FSimplifiedProjectile() : facingRight(true), lifeTime(0), transform() {}
FSimplifiedProjectile::FSimplifiedProjectile(AHitBoxGroupProjectile* model) : facingRight(model->facingRight), lifeTime(model->LifeTime), transform(model->GetActorTransform()) {}
FSimplifiedProjectile::~FSimplifiedProjectile() {}