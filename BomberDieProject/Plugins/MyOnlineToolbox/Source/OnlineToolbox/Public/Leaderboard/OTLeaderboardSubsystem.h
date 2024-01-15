// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "OTLeaderboardSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnLeaderboardReadCompleteDelegate, bool, bWasSuccessful, TArray<FOTLeaderboardResult>, Scores);
/**
 * 
 */
UCLASS()
class ONLINETOOLBOX_API UOTLeaderboardSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UOTLeaderboardSubsystem();

	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintCallable)
	void ReadFriendLeaderboard();

	UFUNCTION(BlueprintCallable)
	void WriteLeaderboard();

	

	

private:
	void HandleLeaderboardResult(const bool bWasSuccessful, FOnlineLeaderboardReadRef LeaderboardRef);

public:
	UPROPERTY(BlueprintAssignable)
	FOTOnLeaderboardReadCompleteDelegate OTOnLeaderboardReadCompleteDelegate;
private:
	IOnlineSubsystem * Subsystem = nullptr;

	IOnlineLeaderboardsPtr OnlineLeaderboards;


	FOnLeaderboardReadCompleteDelegate OnLeaderboardReadCompleteDelegate;
	
	FDelegateHandle OnLeaderboardReadDelegateHandle;
	
	
};

