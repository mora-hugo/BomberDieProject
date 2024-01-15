// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineStatsInterface.h"
#include "Stats/StatReadResult.h"
#include "OTStatisticsSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsUpdatedDelegate,bool,bIsSuccessed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatsQueriedDelegate, bool,bIsSuccessed, const TArray<FStatReadResult>&, StatsQueried);

UCLASS()
class ONLINETOOLBOX_API UOTStatisticsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UOTStatisticsSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void ModifyStats(FString StatName,int32 value, bool AutoSendRequest = true);
	UFUNCTION(BlueprintCallable)
	void SendRequest();
	UFUNCTION(BlueprintCallable)
	void QueryStats(const TArray<FString> & StatsName);
protected:

	//Own delegate
	UPROPERTY(BlueprintAssignable)
	FOnStatsUpdatedDelegate OnStatsUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatsQueriedDelegate OnStatsQueriedDelegate;
	
	FOnlineStatsUpdateStatsComplete OnlineStatsUpdateStatsCompleteDelegate;

	FOnlineStatsQueryUsersStatsComplete OnlineStatsQueryUsersStatsCompleteDelegate;
private:

	void OnOnlineStatsUpdated(const FOnlineError& Error);
	void OnOnlineStatsQueried( const FOnlineError& ResultState, const TArray<TSharedRef<const FOnlineStatsUserStats>>& UsersStatsResult);
	IOnlineIdentityPtr IdentityInterface;
	IOnlineStatsPtr StatsInterface;
	TArray<FOnlineStatsUserUpdatedStats> StatsCached;

};
