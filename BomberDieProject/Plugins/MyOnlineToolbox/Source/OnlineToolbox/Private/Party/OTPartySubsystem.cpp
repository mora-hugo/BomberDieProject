// Fill out your copyright notice in the Description page of Project Settings.


#include "Party/OTPartySubsystem.h"

#include "Interfaces/OnlineIdentityInterface.h"

void UOTPartySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Subsystem = IOnlineSubsystem::Get();
	PartyInterface = Subsystem->GetPartyInterface();
	IdentityInterface = Subsystem->GetIdentityInterface();

}

void UOTPartySubsystem::Deinitialize()
{
	Super::Deinitialize();
}


void UOTPartySubsystem::CreateParty(int PartyId)
{
	TSharedRef<FPartyConfiguration> Config = MakeShared<FPartyConfiguration>();
	Config->bIsAcceptingMembers = true;
	Config->MaxMembers = 4;             // The maximum number of players in the party.
	Config->InvitePermissions = PartySystemPermissions::EPermissionType::Anyone;

	if (!PartyInterface->CreateParty(
			*IdentityInterface->GetUniquePlayerId(0).Get(),  // The local player creating the party.
			(FOnlinePartyTypeId)PartyId,        // The party type ID.
			*Config,
			FOnCreatePartyComplete::CreateLambda([](
				const FUniqueNetId &LocalUserId,
				const TSharedPtr<const FOnlinePartyId> &PartyId,
				const ECreatePartyCompletionResult Result)
				{
					// If Result == ECreatePartyCompletionResult::Succeeded, the party
					// was created and you are now the party leader.
					UE_LOG(LogTemp, Warning, TEXT("Party created"));
				})))
	{
		UE_LOG(LogTemp, Warning, TEXT("error"));
	}

}
