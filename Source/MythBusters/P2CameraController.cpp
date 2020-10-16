// Fill out your copyright notice in the Description page of Project Settings.


#include "P2CameraController.h"
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
	
}


// Called every frame
void UP2CameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	GetOwner()->SetActorLocation(newCameraLocation);
}

