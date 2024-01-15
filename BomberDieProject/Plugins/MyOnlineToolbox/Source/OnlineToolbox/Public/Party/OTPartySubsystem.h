// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlinePartyInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OTPartySubsystem.generated.h"

/**

 */
UCLASS()
class ONLINETOOLBOX_API UOTPartySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void CreateParty(int PartyId);

private:

	IOnlineSubsystem *  Subsystem;

	IOnlinePartyPtr PartyInterface;

	IOnlineIdentityPtr IdentityInterface;
};
