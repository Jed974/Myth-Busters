// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena.h"

// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Root->SetupAttachment(GetRootComponent());

	for (int i = 0; i < 36; i++) {
		FString _componentName = "Ground_";
		_componentName.AppendInt(i);
		Grounds.Add(CreateDefaultSubobject<UArenaElement>(FName(_componentName)));
		Grounds[i]->SetupAttachment(Root);
		Grounds[i]->SetId(i);
	}


	FoliageRoot = CreateDefaultSubobject<USceneComponent>("Foliage");
	FoliageRoot->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AArena::IsEjected(float _velocityNorm, FHitResult& _hitResult)
{
	if (_velocityNorm >= MinimalSpeedToDie) {
		UArenaElement* _meshComponent = Cast<UArenaElement>(_hitResult.GetComponent());
		int idHit = 0;
		if (_meshComponent != nullptr)
			idHit = _meshComponent->GetId();
		else
		{
			// Cast Failed
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Error casting component hit into UArenaElement");
			return false;
		}

		Grounds[idHit]->Explode();
		if (idHit == 0)
			Grounds[35]->Explode();
		else
			Grounds[idHit-1]->Explode();


		if (idHit == 35)
			Grounds[0]->Explode();
		else
			Grounds[idHit + 1]->Explode();
				
		FActorSpawnParameters _spawnParams;
		GetWorld()->SpawnActor<AActor>(explodeActor, FVector::ZeroVector, Grounds[idHit]->GetRelativeRotation(), _spawnParams);

		return true;
	}
	else
		return false;
}