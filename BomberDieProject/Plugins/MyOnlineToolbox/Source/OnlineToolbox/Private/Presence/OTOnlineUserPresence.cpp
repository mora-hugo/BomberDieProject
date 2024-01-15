// Fill out your copyright notice in the Description page of Project Settings.


#include "Presence/OTOnlineUserPresence.h"


EOnlinePresenceState::Type FOTOnlineUserPresence::GetPresenceStateFromCustomPresenceState(
	EOTOnlinePresenceStatus CustomPresenceState)
{
	switch (CustomPresenceState)
	{
	case EOTOnlinePresenceStatus::Online:
		return EOnlinePresenceState::Online;
	case EOTOnlinePresenceStatus::Offline:
		return EOnlinePresenceState::Offline;
	case EOTOnlinePresenceStatus::Away:
		return EOnlinePresenceState::Away;
	case EOTOnlinePresenceStatus::ExtendedAway:
		return EOnlinePresenceState::ExtendedAway;
	case EOTOnlinePresenceStatus::DoNotDisturb:
		return EOnlinePresenceState::DoNotDisturb;
	case EOTOnlinePresenceStatus::Chat:
		return EOnlinePresenceState::Chat;
	default: ;
		return EOnlinePresenceState::Offline;
	}
	
}

EOTOnlinePresenceStatus FOTOnlineUserPresence::GetCustomPresenceStateFromPresenceState(
	EOnlinePresenceState::Type PresenceState)
{
	// Fait moi l'inverse du switch de ma fonction GetPresenceStateFromCustomPresenceState
	switch (PresenceState)
	{
	case EOnlinePresenceState::Type::Online:
		return EOTOnlinePresenceStatus::Online;
	case EOnlinePresenceState::Type::Offline:
		return EOTOnlinePresenceStatus::Offline;
	case EOnlinePresenceState::Type::Away:
		return EOTOnlinePresenceStatus::Away;
	case EOnlinePresenceState::Type::ExtendedAway:
		return EOTOnlinePresenceStatus::ExtendedAway;
	case EOnlinePresenceState::Type::DoNotDisturb:
		return EOTOnlinePresenceStatus::DoNotDisturb;
	case EOnlinePresenceState::Type::Chat:
		return EOTOnlinePresenceStatus::Chat;
	default: ;
		return EOTOnlinePresenceStatus::Offline;
	}
}

FOTOnlineUserPresence FOTOnlineUserPresence::FromOnlinePresence(const FOnlineUserPresenceStatus& Presence)
{
	FOTOnlineUserPresence UserPresence;
	UserPresence.StatusState = GetCustomPresenceStateFromPresenceState(Presence.State);
	UserPresence.Status = Presence.StatusStr;
	return UserPresence;
}

FOnlineUserPresenceStatus FOTOnlineUserPresence::ToOnlinePresence(const FOTOnlineUserPresence& Presence)
{
	FOnlineUserPresenceStatus UserPresence;
	UserPresence.State = GetPresenceStateFromCustomPresenceState(Presence.StatusState);
	UserPresence.StatusStr = Presence.Status;
	return UserPresence;
}
