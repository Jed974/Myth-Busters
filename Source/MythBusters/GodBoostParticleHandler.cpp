// Fill out your copyright notice in the Description page of Project Settings.


#include "god.h"
#include "GodBoostParticleHandler.h"

void UGodBoostParticleHandler::SetUpGod(AActor* owner) {
	godOwner = Cast<AGod>(owner);
	InitArray();
}
