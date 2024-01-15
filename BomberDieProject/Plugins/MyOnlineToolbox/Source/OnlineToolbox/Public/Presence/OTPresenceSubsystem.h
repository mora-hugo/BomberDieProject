// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlinePresenceStatus.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OTOnlineUserPresence.h"
#include "Friends/OTUser.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "OTPresenceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPresenceUpdated, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPresenceQueryied, bool, bWasSuccessful, const TArray<FOTOnlineUserPresence>&, Presences);

/**
 * 
 */
UCLASS()
class ONLINETOOLBOX_API UOTPresenceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	
	UOTPresenceSubsystem();
	
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;
public:
	
	UFUNCTION(BlueprintCallable)
	void UpdatePresence(const FOTOnlineUserPresence& Presence);

	
	
	UFUNCTION(BlueprintCallable)
	void QueryPresences(const TArray<FOTUser>& Users);

	UFUNCTION(BlueprintCallable)
	void GetCachedPresences(TArray<FOTOnlineUserPresence>& OutPresences);

private:
	void OnPresenceUpdatedComplete(const FUniqueNetId& UniqueNetId, bool bWasSuccessful);
	
	void OnPresenceQueryiedComplete(const FUniqueNetId& UniqueNetId, bool bWasSuccessful);
public:
	UPROPERTY(BlueprintAssignable)
	FOnPresenceUpdated OnPresenceUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPresenceQueryied OnPresenceQueryiedDelegate;
	
protected:
	
	TArray<FOTUser> LastUsersQueried;
	
private:
	IOnlineSubsystem* Subsystem;

	IOnlinePresencePtr PresenceInterface;

	IOnlineIdentityPtr IdentityInterface;



	/* Online subsystem delegates */
	IOnlinePresence::FOnPresenceTaskCompleteDelegate OnPresenceUpdatedCompleteDelegate;

	IOnlinePresence::FOnPresenceTaskCompleteDelegate OnPresenceQueryCompleteDelegate;

	
	
	
};
