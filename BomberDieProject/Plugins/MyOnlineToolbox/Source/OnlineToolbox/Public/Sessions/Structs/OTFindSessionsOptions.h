// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OTFindSessionsOptions.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTFindSessionOptions
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsInLAN;
	
	UPROPERTY(BlueprintReadWrite)
	FString SessionName;
	
	UPROPERTY(BlueprintReadWrite)
	FString HostName;
	
	UPROPERTY(BlueprintReadWrite)
	FString Map;
	
	UPROPERTY(BlueprintReadWrite)
	FString SessionType;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MinSlot;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxSlot;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxSearchResults;

	

 

 
};