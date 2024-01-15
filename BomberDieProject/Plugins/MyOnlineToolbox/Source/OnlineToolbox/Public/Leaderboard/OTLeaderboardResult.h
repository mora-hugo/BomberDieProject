#pragma once

#include "CoreMinimal.h"
#include "Friends/OTUser.h"
#include "OTLeaderboardResult.generated.h"


USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTLeaderboardResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FOTUser User;

	UPROPERTY(BlueprintReadOnly)
	int32 Rank;

	UPROPERTY(BlueprintReadOnly)
	int32 Score;

	
	
};
    