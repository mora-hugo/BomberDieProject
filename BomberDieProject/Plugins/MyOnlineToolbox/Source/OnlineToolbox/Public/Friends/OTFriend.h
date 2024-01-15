// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OTUser.h"
#include "OTFriend.generated.h"

class FOnlineFriend;
/**
 * A wrapper for the class FOnlineFriend 
 */
USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTFriend
{
	GENERATED_BODY()
	TSharedPtr<FOnlineFriend> OnlineFriend;

	UPROPERTY(BlueprintReadOnly,Category="Friend Informations")
	FOTUser User = FOTUser();

	UPROPERTY(BlueprintReadOnly)
	UTexture2D * Avatar;

	
};
