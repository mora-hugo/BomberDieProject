// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "OTUser.h"
#include "OTBlockedUser.generated.h"


USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTBlockedUser
{
	GENERATED_BODY()
	
	TSharedPtr<FOnlineBlockedPlayer> OnlineBlockUser;

	FOTUser User;


	
};