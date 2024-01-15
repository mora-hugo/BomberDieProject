// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OTAchievement.generated.h"

USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTAchievement
{
	GENERATED_BODY()

	FString Id = FString();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements", meta=(ClampMin=0, ClampMax=100))
	double Completion = 0.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Achievements")
	FText Title = FText();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Achievements")
	FText LockedDesc = FText();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Achievements")
	FText UnlockedDesc = FText();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Achievements")
	bool bIsHidden = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Achievements")
	FDateTime UnlockTime;

	FString ToDebugString() const
	{
		return FString::Printf( TEXT("Title='%s', LockedDesc='%s', UnlockedDesc='%s', bIsHidden=%s, UnlockTime=%s"),
			*Title.ToString(),
			*LockedDesc.ToString(),
			*UnlockedDesc.ToString(),
			bIsHidden ? TEXT("true") : TEXT("false"),
			*UnlockTime.ToString()
			);
	}
};
