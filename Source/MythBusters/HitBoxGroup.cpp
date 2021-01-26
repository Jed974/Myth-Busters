// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBoxGroup.h"

// Sets default values
AHitBoxGroup::AHitBoxGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHitBoxGroup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitBoxGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetActorLocation(FVector(GetActorLocation().X, 0.f, GetActorLocation().Z));
	//SetActorRotation(FRotator(0.f, 0.f, GetActorRotation().Roll));

}


bool AHitBoxGroup::GodHitIsValid(AGod* godHit) {
	if (godHit == GetInstigator())
		return false;
	else if (alreadyHit)//(AlreadyHitGods.Contains(godHit))
		return false;
	else
		return true;
}
/*void AHitBoxGroup::RegisterGodHit(AGod* godHit) {
	AlreadyHitGods.Add(godHit);
}*/
