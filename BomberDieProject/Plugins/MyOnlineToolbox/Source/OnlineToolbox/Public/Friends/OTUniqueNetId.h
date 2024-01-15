#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "OTUniqueNetId.generated.h"

USTRUCT(BlueprintType)
struct ONLINETOOLBOX_API FOTUniqueNetId
{
	GENERATED_BODY()

	FOTUniqueNetId(){};
	FOTUniqueNetId(TSharedPtr<const FUniqueNetId> InUniqueNetId) : NativeUniqueNetId(InUniqueNetId), UniqueNetId(InUniqueNetId->ToString()) {}

	
	TSharedPtr<const FUniqueNetId> NativeUniqueNetId;

	UPROPERTY(BlueprintReadOnly)
	FString UniqueNetId;
};