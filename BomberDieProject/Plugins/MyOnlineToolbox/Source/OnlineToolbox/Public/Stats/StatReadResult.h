// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatReadResult.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FStatReadResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString PlayerId = FString();
	
	UPROPERTY(BlueprintReadOnly)
	FString StatName = FString();
	
	UPROPERTY(BlueprintReadOnly)
	int32 StatValue = 0;
};
