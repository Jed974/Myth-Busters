// Fill out your copyright notice in the Description page of Project Settings.


#include "P2CameraController.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "god.h"
#include "GenericPlatform/GenericPlatformMath.h"


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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGod::StaticClass(), Gods);
}


// Called every frame
void UP2CameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Barycenter = FVector(0.f, 0.f, 0.f);
	for (AActor* God : Gods)
	{
		Barycenter += God->GetActorLocation();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(Gods.Max()));
	Barycenter *= 2.f/ Gods.Max(); //Bugfix temporaire !!!

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Barycenter.ToString());

	/*float GreatestGodDistanceToBarycenter = 0.f;
	for (AActor* God : Gods)
	{
		if (FVector::Dist(God->GetActorLocation(), Barycenter) > GreatestGodDistanceToBarycenter)
		{
			GreatestGodDistanceToBarycenter = FVector::Dist(God->GetActorLocation(), Barycenter);
		}
	}


	GetOwner()->SetActorLocation(FVector(Barycenter.X, initialCameraDistance + GreatestGodDistanceToBarycenter*1.5f, Barycenter.Z));*/

	float GreatestGodDistanceToBarycenter = 0;
	for (AActor* God : Gods)
	{
		if ((God->GetActorLocation() - Barycenter).X > GreatestGodDistanceToBarycenter)
		{
			GreatestGodDistanceToBarycenter = (God->GetActorLocation() - Barycenter).X;
		}
		if ((God->GetActorLocation() - Barycenter).Z * 1.78 > GreatestGodDistanceToBarycenter)
		{
			GreatestGodDistanceToBarycenter = (God->GetActorLocation() - Barycenter).Z *1.78;
		}
	}

	GetOwner()->SetActorLocation(FVector(Barycenter.X, initialCameraDistance + GreatestGodDistanceToBarycenter, Barycenter.Z)); //GreatestGodDistanceToBarycenter.Y*1.77 ou 1.77 est l'aspect ratio de la camera
}

