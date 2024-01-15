// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaderboard/OTLeaderboardSubsystem.h"

#include <Steamworks/Steamv153/sdk/public/steam/isteaminventory.h>

#include "Friends/OTUser.h"
#include "Leaderboard/OTLeaderboardResult.h"

UOTLeaderboardSubsystem::UOTLeaderboardSubsystem() 
{
	
}

void UOTLeaderboardSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem, TEXT("Unable to get the online subsystem"));

	OnlineLeaderboards = Subsystem->GetLeaderboardsInterface();
	if(!ensureMsgf(OnlineLeaderboards, TEXT("Unable to get the online leaderboard"))) return;
	
	
}

void UOTLeaderboardSubsystem::ReadFriendLeaderboard()
{
	FOnlineLeaderboardReadRef ReadRef = MakeShared<FOnlineLeaderboardRead>();
	ReadRef->LeaderboardName = FName(TEXT("BoardLevelEasy"));
	
	OnLeaderboardReadDelegateHandle = OnlineLeaderboards->AddOnLeaderboardReadCompleteDelegate_Handle(
		FOnLeaderboardReadComplete::FDelegate::CreateUObject(
		this,
		&ThisClass::HandleLeaderboardResult,
		ReadRef));

	
	
	ReadRef->ColumnMetadata.Add(FColumnMetaData(FName(TEXT("ATTEMPTEASY")), EOnlineKeyValuePairDataType::Int32));
	ReadRef->SortedColumn = FName(TEXT("ATTEMPTEASY"));

	if(!OnlineLeaderboards->ReadLeaderboardsForFriends(0, ReadRef))
	{
		OnlineLeaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(OnLeaderboardReadDelegateHandle);
		OTOnLeaderboardReadCompleteDelegate.Broadcast(false, {});
	}


}

void UOTLeaderboardSubsystem::WriteLeaderboard()
{
}

void UOTLeaderboardSubsystem::HandleLeaderboardResult(const bool bWasSuccessful,
                                                      FOnlineLeaderboardReadRef LeaderboardRef)
{
	OnlineLeaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(OnLeaderboardReadDelegateHandle);

	if(!bWasSuccessful)
	{
		OTOnLeaderboardReadCompleteDelegate.Broadcast(false, {});
		return;
	}

	TArray<FOTLeaderboardResult> Results;
	for (auto Row : LeaderboardRef->Rows)
	{
		//Get user info
		FOTUser User;
		User.Name = Row.NickName;
		User.UniqueNetId = FOTUniqueNetId(Row.PlayerId);
		
		FOTLeaderboardResult Result;
		Result.User = User;

		// Set the score in our struct
		int32 Score;
		Row.Columns[TEXT("ATTEMPTEASY")].GetValue(Score);
		Result.Score = Score;

		Result.Rank = Row.Rank;

		Results.Add(Result);
	}

	OTOnLeaderboardReadCompleteDelegate.Broadcast(true, Results);
}
