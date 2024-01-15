// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OTChatGroup.generated.h"


UENUM(BlueprintType)
enum class EChatGroup : uint8
{
	All UMETA(DisplayName = "All"),
	Friends UMETA(DisplayName = "Friends"),
	Party UMETA(DisplayName = "Party"),
	Team UMETA(DisplayName = "Team"),
	Private UMETA(DisplayName = "Private"),
};

