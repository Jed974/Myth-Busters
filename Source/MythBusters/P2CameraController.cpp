// Fill out your copyright notice in the Description page of Project Settings.


#include "P2CameraController.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "god.h"


// Sets default values for this component's properties
UP2CameraController::UP2CameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UP2CameraController::BeginPlay()
{
	Super::BeginPlay();
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGod::StaticClass(), Gods);
}


// Called every frame
void UP2CameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*FVector Barycenter = FVector(0.f, 0.f, 0.f);
	for (AActor* God : Gods)
	{
		Barycenter += God->GetActorLocation();
	}
	
	Barycenter *= 1.f/ Gods.Max();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Barycenter.ToString());

	float GreatestGodDistanceToBarycenter = 0.f;
	for (AActor* God : Gods)
	{
		if (FVector::Dist(God->GetActorLocation(), Barycenter) > GreatestGodDistanceToBarycenter)
		{
			GreatestGodDistanceToBarycenter = FVector::Dist(God->GetActorLocation(), Barycenter);
		}
	}*/

	//GetOwner()->SetActorLocation(FVector(Barycenter.X, initialCameraDistance + GreatestGodDistanceToBarycenter*1.5f, Barycenter.Z));

	/*
	//milieu
	FVector locationPlayer1 = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector locationPlayer2 = FVector(0,0,0);
	FVector milieuP1P2 = (locationPlayer1 + locationPlayer2 ) / 2;


	//zoom
	float reculAxeX = abs(locationPlayer1.X - locationPlayer2.X);
	float reculAxeZ = abs(locationPlayer1.Z - locationPlayer2.Z);
	float CameraYLocation;
	if (reculAxeX > reculAxeZ) {
			CameraYLocation = reculAxeX;
	}
	else
	{
			CameraYLocation = reculAxeZ;
	}



	//deplacement vers la nouvelle position
	FVector newCameraLocation = FVector(milieuP1P2.X, CameraYLocation + initialCameraDistance, milieuP1P2.Z);
	GetOwner()->SetActorLocation(newCameraLocation);*/
}

