// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OTCreateSessionsOptions.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTCreateSessionsOptions
{
 GENERATED_BODY()
 
 UPROPERTY(BlueprintReadWrite)
 bool bIsInLAN = false;
 
 UPROPERTY(BlueprintReadWrite)
 FString SessionName = FString();
 
 UPROPERTY(BlueprintReadWrite)
 FString HostName = FString();
 
 UPROPERTY(BlueprintReadWrite)
 FString Map = FString();
 
 UPROPERTY(BlueprintReadWrite)
 FString SessionType = FString();
 
 UPROPERTY(BlueprintReadWrite)
 int NumPublicConnections = 4;

 

 
};
