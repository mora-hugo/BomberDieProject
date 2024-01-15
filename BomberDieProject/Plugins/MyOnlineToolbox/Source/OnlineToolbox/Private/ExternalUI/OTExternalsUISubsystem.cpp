// Fill out your copyright notice in the Description page of Project Settings.
#include "ExternalUI/OTExternalsUISubsystem.h"

#include "Identity/OTIdentitySubsystem.h"
#include "Interfaces/OnlineExternalUIInterface.h"


UOTExternalsUISubsystem::UOTExternalsUISubsystem() : OnProfileClosedDelegate(FOnProfileUIClosedDelegate::CreateUObject(this, &UOTExternalsUISubsystem::HandleProfileUIClosed))
{
}

void UOTExternalsUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem != nullptr, TEXT("Unable to get the SubSytem"));

	ExternalUIInterface = Subsystem->GetExternalUIInterface();
	if(!ensureMsgf(ExternalUIInterface != nullptr, TEXT("Unable to get the External UI interface"))) return;
	
}

void UOTExternalsUISubsystem::ShowProfileUI(const FOTUniqueNetId& UniqueNetId)
{
	//Get OTIdentitySubsystem
	UOTIdentitySubsystem * IdentitySubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UOTIdentitySubsystem>();
	if(!ensureMsgf(IdentitySubsystem != nullptr, TEXT("Unable to get the Identity Subsystem"))) return;
	ExternalUIInterface->ShowProfileUI(IdentitySubsystem->GetLocalUniqueNetId().NativeUniqueNetId->AsShared().Get(),UniqueNetId.NativeUniqueNetId->AsShared().Get(),OnProfileClosedDelegate);
}

void UOTExternalsUISubsystem::ShowFriendUI(const FOTUniqueNetId& UniqueNetId)
{
	ExternalUIInterface->ShowFriendsUI(0);
}

void UOTExternalsUISubsystem::ShowInviteUI(const FOTUniqueNetId& UniqueNetId)
{
	ExternalUIInterface->ShowInviteUI(0);
}
void UOTExternalsUISubsystem::HandleProfileUIClosed()
{
	OnOTProfileUIClosedDelegate.Broadcast();
}
