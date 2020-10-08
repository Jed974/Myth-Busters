// Fill out your copyright notice in the Description page of Project Settings.


#include "god.h"

// Sets default values
Agod::Agod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	GodMovement = CreateDefaultSubobject<UGodMovementComponent>("GodMovementComponent");

}

// Called when the game starts or when spawned
void Agod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Agod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

