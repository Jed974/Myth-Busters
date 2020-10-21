// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBoxGroupProjectile.h"

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
}