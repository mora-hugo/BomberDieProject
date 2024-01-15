// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlinePresenceStatus.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "OTOnlineUserPresence.generated.h"
USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTOnlineUserPresence
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite,Category="Presence Informations")
	EOTOnlinePresenceStatus StatusState = EOTOnlinePresenceStatus::Offline;

	UPROPERTY(BlueprintReadWrite,Category="Presence Informations")
	FString Status = FString();

	UPROPERTY(BlueprintReadOnly,Category="Presence Informations")
	FString AppId = FString("480");

	static EOnlinePresenceState::Type GetPresenceStateFromCustomPresenceState(EOTOnlinePresenceStatus CustomPresenceState);

	static EOTOnlinePresenceStatus GetCustomPresenceStateFromPresenceState(EOnlinePresenceState::Type PresenceState);

	static FOTOnlineUserPresence FromOnlinePresence(const FOnlineUserPresenceStatus& Presence);

	static FOnlineUserPresenceStatus ToOnlinePresence(const FOTOnlineUserPresence& Presence);

	

	
	
};
