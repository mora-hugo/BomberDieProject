// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "BSGameInstance.h"
#include "GameFramework/Actor.h"
#include "BaseGameInstance.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API UBaseGameInstance : public UBSGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Network")
	TSoftObjectPtr<UWorld> MainMenuLevel;

private:
	FDelegateHandle NetworkFailureHandler;
	void HandleNetworkFailure(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & Error);
	
	FDelegateHandle TravelFailureHandler;
	void HandleTravelError(UWorld *World, ETravelFailure::Type TravelFailure, const FString& Error);

	FString LastError;
	void ClearSession();
	void OnDestroySessionCompleteHandle(FName SessionName, bool bWasSuccessful) const;
};
