// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OTSettings.generated.h"

/**
 * 
 */
UCLASS(config = OnlineToolboxSettings)
class ONLINETOOLBOX_API UOTSettings : public UObject
{
	GENERATED_BODY()
public:
	UOTSettings(const FObjectInitializer& obj);
	
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray<FName> Statistics;

};
