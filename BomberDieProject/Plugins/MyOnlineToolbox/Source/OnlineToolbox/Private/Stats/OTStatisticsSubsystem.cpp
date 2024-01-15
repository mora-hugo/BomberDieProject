// Fill out your copyright notice in the Description page of Project Settings.

#include "Stats/OTStatisticsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Identity/OTIdentitySubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Stats/StatReadResult.h"

UOTStatisticsSubsystem::UOTStatisticsSubsystem() : OnlineStatsUpdateStatsCompleteDelegate(FOnlineStatsUpdateStatsComplete::CreateUObject(this, &UOTStatisticsSubsystem::OnOnlineStatsUpdated))
{
	
}

void UOTStatisticsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem != nullptr, TEXT("Unable to get the SubSytem"));

	
	StatsInterface = Subsystem->GetStatsInterface();
	if(!ensureMsgf(StatsInterface, TEXT("Unable to get the Stats Interface"))) return;

	IdentityInterface = Subsystem->GetIdentityInterface();
	if(!ensureMsgf(IdentityInterface, TEXT("Unable to get the Identity interface"))) return;
}

void UOTStatisticsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UOTStatisticsSubsystem::ModifyStats(FString StatName, int32 value, bool AutoSendRequest)
{
	const auto UniquePlayerId = IdentityInterface->GetUniquePlayerId(0);
	FOnlineStatsUserUpdatedStats Stat = FOnlineStatsUserUpdatedStats(UniquePlayerId.ToSharedRef());
	Stat.Stats.Add(StatName, FOnlineStatUpdate(value, FOnlineStatUpdate::EOnlineStatModificationType::Unknown));

	//Add stat to array
	StatsCached.Add(Stat);
	
	if(AutoSendRequest)
		SendRequest();
    }


void UOTStatisticsSubsystem::SendRequest()
{
	const auto UserId = IdentityInterface->GetUniquePlayerId(0).ToSharedRef();
	StatsInterface->UpdateStats(UserId,StatsCached,OnlineStatsUpdateStatsCompleteDelegate);
	StatsCached.Reset();

}

void UOTStatisticsSubsystem::QueryStats(const TArray<FString> & StatsName)
{
	
	const TSharedPtr<const FUniqueNetId> UniqueId = IdentityInterface->GetUniquePlayerId(0).ToSharedRef();
	StatsInterface->QueryStats(
	IdentityInterface->GetUniquePlayerId(0).ToSharedRef(),
	{UniqueId.ToSharedRef()},
	StatsName,FOnlineStatsQueryUsersStatsComplete::CreateUObject(this,&ThisClass::OnOnlineStatsQueried)
	);
}

void UOTStatisticsSubsystem::OnOnlineStatsUpdated(const FOnlineError& Error)
{
	OnStatsUpdatedDelegate.Broadcast(Error.bSucceeded);
}

void UOTStatisticsSubsystem::OnOnlineStatsQueried( const FOnlineError& ResultState, const TArray<TSharedRef<const FOnlineStatsUserStats>>& UsersStatsResult)
{
	TArray<FStatReadResult> StatsRead;
	if (ResultState.bSucceeded && UsersStatsResult.Num() > 0) {
		for(auto OnlineUserStat : UsersStatsResult)
		{
			for (const auto QueriedStats : UsersStatsResult)
			{
				// Iterate through the map to retrieve stats
				for (auto KV : QueriedStats->Stats)
				{
					FStatReadResult StatRead;
					StatRead.StatName = KV.Key;
					KV.Value.GetValue(StatRead.StatValue);
					StatRead.PlayerId = QueriedStats->Account->ToString();
					StatsRead.Add(StatRead);
				}
			}
		}
	}
	
	OnStatsQueriedDelegate.Broadcast(ResultState.bSucceeded,StatsRead);

	
	
	
	
}
