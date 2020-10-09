// Fill out your copyright notice in the Description page of Project Settings.


#include "P2CameraController.h"

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

	// ...
	
}


// Called every frame
void UP2CameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//milieu
	FVector locationPlayer1 = Player1->GetActorLocation();
	FVector locationPlayer2 = Player2->GetActorLocation();


	FVector milieuP1P2 = (locationPlayer1 + locationPlayer2 ) / 2;


	//zoom
	float tempX = abs(locationPlayer1.X - locationPlayer2.X) / 2;
	float reculAxeX = tempX/2 - tempX*tempX/4;
	float tempZ = abs(locationPlayer1.Z - locationPlayer2.Z) / 2;
	float reculAxeZ = tempZ / 2 - tempZ * tempZ / 4;
	float CameraYLocation;
	if (reculAxeX > reculAxeZ) {
			CameraYLocation = reculAxeX;
	}
	else
	{
			CameraYLocation = reculAxeZ;
	}
	FString CameraYLocationStr = FString::SanitizeFloat(CameraYLocation);
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("noucellePos:"));
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *CameraYLocationStr);*/


	//deplacement vers la nouvelle position
	FVector newCameraLocation = FVector(milieuP1P2.X, - CameraYLocation - initialCameraDistance, milieuP1P2.Z);
	//En commentaire tant que j'ai pas fini
	//GetOwner()->SetActorLocation(newCameraLocation);
}

