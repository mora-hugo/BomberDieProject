// Fill out your copyright notice in the Description page of Project Settings.
#include "Achievements/OTAchievementsSubsystem.h"

#include "Achievements/OTAchievement.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

UOTAchievementsSubsystem::UOTAchievementsSubsystem():
	QueryAchievementsCompleteDelegate(FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UOTAchievementsSubsystem::HandleQueryAchievements)),
	QueryAchievementsDescCompleteDelegate(FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UOTAchievementsSubsystem::HandleQueryAchievementsDesc))
	//AchievementsWrittenDelegate(FOnAchievementsWrittenDelegate::CreateUObject(this, &UOTAchievementsSubsystem::HandleAchievementWritten)),
	//AchievementUnlockedDelegate(FOnAchievementUnlockedDelegate::CreateUObject(this, &UOTAchievementsSubsystem::HandleAchievementUnlocked))
{
}

void UOTAchievementsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const auto* Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem != nullptr, TEXT("Unable to get the SubSystem"));

	AchievementsInterface = Subsystem->GetAchievementsInterface();
	checkf(AchievementsInterface.IsValid(), TEXT("Unable to get the Session Interface"));

	IdentityInterface = Subsystem->GetIdentityInterface();
	checkf(IdentityInterface.IsValid(), TEXT("Unable to get the Identity Interface"));

}

void UOTAchievementsSubsystem::Deinitialize()
{
	Super::Deinitialize();

}

void UOTAchievementsSubsystem::UpdateAchievements()
{
	checkf(AchievementsInterface.IsValid(), TEXT("Unable to get the Session Interface"));

	if(bUpdatingAchievements) return;
	bUpdatingAchievements = true;
	
	//Get the local player because we need the Unique Net ID
	//const auto LocalPLayerNetId = GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId();
	const auto* Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem != nullptr, TEXT("Unable to get the SubSytem"));

	
	AchievementsInterface->QueryAchievements(*IdentityInterface->GetUniquePlayerId(0), QueryAchievementsCompleteDelegate);
}

void UOTAchievementsSubsystem::ClearAchievements()
{
#if !UE_BUILD_SHIPPING
	checkf(AchievementsInterface.IsValid(), TEXT("Unable to get the Session Interface"));
	AchievementsInterface->ResetAchievements(*IdentityInterface->GetUniquePlayerId(0));
#endif
}

bool UOTAchievementsSubsystem::GetAchievementFromCache(const FString& AchievementId,
	FOTAchievement& OutAchievement) const
{
	checkf(AchievementsInterface.IsValid(), TEXT("Unable to get the Session Interface"));

	//Get the completion
	FOnlineAchievement CachedAchievement;
	auto result = AchievementsInterface->GetCachedAchievement(*IdentityInterface->GetUniquePlayerId(0), AchievementId, CachedAchievement);
	if(result == EOnlineCachedResult::NotFound)
	{
		FOTAchievementError Error;
		Error.ErrorCode = EOTAchievementErrorCode::NotFound;
		Error.ErrorMessage = "Achievement not found in cache. Try to update your achievements.";
		OnAchievementsError.Broadcast(Error);
		return false;
	}
		
	//Get the description
	FOnlineAchievementDesc Desc;
	result = AchievementsInterface->GetCachedAchievementDescription(AchievementId, Desc);
	if(result == EOnlineCachedResult::NotFound)
	{
		FOTAchievementError Error;
		Error.ErrorCode = EOTAchievementErrorCode::NotFound;
		Error.ErrorMessage = "Achievement not found in cache. Try to update your achievements.";
		OnAchievementsError.Broadcast(Error);
		return false;
	}

	//Construct our BP ready achievement
	FOTAchievement Achievement;
	Achievement.Id = CachedAchievement.Id;
	Achievement.Completion = CachedAchievement.Progress;
	Achievement.Title = Desc.Title;
	Achievement.LockedDesc = Desc.LockedDesc;
	Achievement.UnlockedDesc = Desc.UnlockedDesc;
	Achievement.bIsHidden = Desc.bIsHidden;
	Achievement.UnlockTime = Desc.UnlockTime;

	OutAchievement = Achievement;
	return true;
}

bool UOTAchievementsSubsystem::GetAchievementsFromCache(TArray<FOTAchievement>& OutAchievements) const
{
	
	//Get the achievements from the cache
	TArray<FOnlineAchievement> OnlineAchievements;
	AchievementsInterface->GetCachedAchievements(*IdentityInterface->GetUniquePlayerId(0), OnlineAchievements);
	
	for(const auto& Achievement : OnlineAchievements)
	{
		FOTAchievement OutAchievement;
		if(!GetAchievementFromCache(Achievement.Id, OutAchievement))
		{
			continue;
		}
		OutAchievements.Add(OutAchievement);
	}

	return true;
}

void UOTAchievementsSubsystem::HandleQueryAchievements(const FUniqueNetId& PlayerID, const bool bSuccessful) const
{
	if(!bSuccessful)
	{
		FOTAchievementError Error;
		Error.ErrorCode = EOTAchievementErrorCode::Unknown;
		Error.ErrorMessage = "Online Task Failed HandleQueryAchievements";
		OnAchievementsError.Broadcast(Error);
		return;
	}

	//We have the achievements (ID/completion) but we need the achievements descriptions as well (name, desc, image)
	AchievementsInterface->QueryAchievementDescriptions(PlayerID, QueryAchievementsDescCompleteDelegate);
}

void UOTAchievementsSubsystem::HandleQueryAchievementsDesc(const FUniqueNetId& PlayerID, const bool bSuccessful)
{
	bUpdatingAchievements = false;
	
	if(!bSuccessful)
	{
		FOTAchievementError Error;
		Error.ErrorCode = EOTAchievementErrorCode::Unknown;
		Error.ErrorMessage = "Online Task Failed HandleQueryAchievementsDesc";
		OnAchievementsError.Broadcast(Error);
		return;
	}

	//Get the achievements from the cache
	TArray<FOTAchievement> Achievements;
	GetAchievementsFromCache(Achievements);

	//Send all the new achievements
	OnAchievementsUpdated.Broadcast(true, Achievements);

	//Send all the unlocked achievements
	WaitingAchievementsSection.Lock();
	for(const auto& AchievementId : WaitingAchievementsUnlocked)
	{
		FOTAchievement Achievement;
		GetAchievementFromCache(AchievementId, Achievement);
	}
	WaitingAchievementsUnlocked.Empty();
	WaitingAchievementsSection.Unlock();
}
/*
void UOTAchievementsSubsystem::HandleAchievementWritten(const FUniqueNetId& PlayerID, const bool bSuccessful)
{
	if(!bSuccessful)
	{
		FOTAchievementError Error;
		Error.ErrorCode = EOTAchievementErrorCode::Unknown;
		Error.ErrorMessage = "Online Task Failed";
		OnAchievementsError.Broadcast(Error);
		return;
	}

	if(WriteRequest->WriteState == EOnlineAsyncTaskState::Failed)
	{
		FOTAchievementError Error;
		Error.ErrorCode = EOTAchievementErrorCode::Unknown;
		Error.ErrorMessage = "Online Task Failed";
		OnAchievementsError.Broadcast(Error);
		return;
	}

	OnAchievementsWritten.Broadcast(bSuccessful);
	
	//Redownload our cached achievements
	UpdateAchievements();
}

void UOTAchievementsSubsystem::HandleAchievementUnlocked(const FUniqueNetId& UniqueNetId, const FString& AchievementId)
{
	WaitingAchievementsSection.Lock();
	//Add the achievement unlocked to our waiting for unlocking list
	WaitingAchievementsUnlocked.Add(AchievementId);
	WaitingAchievementsSection.Unlock();
	
	//We update the achievements and deal with unlocking code once the query is done
	if(!bUpdatingAchievements)
	{
		UpdateAchievements();
	}
}*/