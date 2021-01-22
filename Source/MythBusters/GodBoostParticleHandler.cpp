// Fill out your copyright notice in the Description page of Project Settings.


#include "GodBoostParticleHandler.h"
#include "god.h"


void UGodBoostParticleHandler::SetUpGod(AActor* owner) {
	godOwner = Cast<AGod>(owner);
	InitArray();
}
