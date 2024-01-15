// Fill out your copyright notice in the Description page of Project Settings.


#include "Presence/OTPresenceSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Interfaces/OnlineSessionInterface.h"


#include "Presence/OnlinePresenceStatus.h"


UOTPresenceSubsystem::UOTPresenceSubsystem() : OnPresenceUpdatedCompleteDelegate(IOnlinePresence::FOnPresenceTaskCompleteDelegate::CreateUObject(this, &UOTPresenceSubsystem::OnPresenceUpdatedComplete)),
												OnPresenceQueryCompleteDelegate(IOnlinePresence::FOnPresenceTaskCompleteDelegate::CreateUObject(this,&UOTPresenceSubsystem::OnPresenceQueryiedComplete))
{
	
}

void UOTPresenceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Subsystem = IOnlineSubsystem::Get();
	IdentityInterface = Subsystem->GetIdentityInterface();
	PresenceInterface = Subsystem->GetPresenceInterface();
}

void UOTPresenceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UOTPresenceSubsystem::UpdatePresence(const FOTOnlineUserPresence& Presence)
{
	const FOnlineUserPresenceStatus UserPresence = FOTOnlineUserPresence::ToOnlinePresence(Presence);
	PresenceInterface->SetPresence(
		*IdentityInterface->GetUniquePlayerId(0).Get(),
		UserPresence,
		OnPresenceUpdatedCompleteDelegate);
}



void UOTPresenceSubsystem::QueryPresences(const TArray<FOTUser>& Users)
{
	TArray<FUniqueNetIdRef> UniqueNetIds;
	for (const FOTUser& User : Users)
	{
		UniqueNetIds.Add(User.UniqueNetId.NativeUniqueNetId.ToSharedRef());
	}
	
	PresenceInterface->QueryPresence(
	*IdentityInterface->GetUniquePlayerId(0).Get(),
	UniqueNetIds,
	OnPresenceQueryCompleteDelegate);
}

void UOTPresenceSubsystem::GetCachedPresences(TArray<FOTOnlineUserPresence>& OutPresences)
{
	TArray<TSharedPtr<FOnlineUserPresence>> Presences;
	for(const FOTUser& User : LastUsersQueried)
	{
		TSharedPtr<FOnlineUserPresence> Presence = MakeShared<FOnlineUserPresence>();
		if(PresenceInterface->GetCachedPresence(
			User.UniqueNetId.NativeUniqueNetId->AsShared().Get(),
			Presence) != EOnlineCachedResult::Success)
				continue;
		Presences.Add(Presence);
	}

	for (const TSharedPtr<FOnlineUserPresence>& Presence : Presences)
	{
		FOnlineUserPresence & Pres = *Presence.Get();
		FOTOnlineUserPresence Status = FOTOnlineUserPresence::FromOnlinePresence(Pres.Status);
		OutPresences.Add(Status);
	}
}

void UOTPresenceSubsystem::OnPresenceUpdatedComplete(const FUniqueNetId& UniqueNetId, bool bWasSuccessful)
{
	OnPresenceUpdatedDelegate.Broadcast(bWasSuccessful);
}

void UOTPresenceSubsystem::OnPresenceQueryiedComplete(const FUniqueNetId& UniqueNetId, bool bWasSuccessful)
{
	TArray<FOTOnlineUserPresence> OutPresences;
	GetCachedPresences(OutPresences);
	OnPresenceQueryiedDelegate.Broadcast(bWasSuccessful, OutPresences);
}

