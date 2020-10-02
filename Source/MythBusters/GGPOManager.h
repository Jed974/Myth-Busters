// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../GGPO/src/include/ggponet.h"

/**
 * 
 */
class MYTHBUSTERS_API GGPOManager
{
public:
	GGPOManager();
	~GGPOManager();

    GGPOErrorCode result;
    GGPOSessionCallbacks cb;

};
