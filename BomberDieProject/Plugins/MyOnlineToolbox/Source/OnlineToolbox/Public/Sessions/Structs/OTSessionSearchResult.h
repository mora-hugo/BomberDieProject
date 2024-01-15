// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OTSessionSearchResult.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTSessionSearchResult
{
	GENERATED_BODY()

	FOnlineSession Session;

	FOnlineSessionSearchResult Native;

	UPROPERTY(BlueprintReadOnly)
	int32 PingInMs;
	
	UPROPERTY(BlueprintReadOnly)
	FString SessionName;
	
	UPROPERTY(BlueprintReadOnly)
	FString SessionType;
	
	UPROPERTY(BlueprintReadOnly)
	FString HostName;
	
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPublicConnection;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPublicConnection;
	
	UPROPERTY(BlueprintReadOnly)
	FString Map;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInLan;
	
};
