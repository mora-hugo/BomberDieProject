// Fill out your copyright notice in the Description page of Project Settings.


#include "Identity/OTIdentitySubsystem.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Engine/PlatformInterfaceBase.h"

UOTIdentitySubsystem::UOTIdentitySubsystem() :
	LoginCompleteDelegate(FOnLoginCompleteDelegate::CreateUObject(this, &UOTIdentitySubsystem::HandleLoginComplete)),
LogoutCompleteDelegate(FOnLogoutCompleteDelegate::CreateUObject(this, &UOTIdentitySubsystem::HandleLogoutComplete)),
LoginChangedDelegate(FOnLoginChangedDelegate::CreateUObject(this, &UOTIdentitySubsystem::HandleLoginChange))
{
}

void UOTIdentitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnlineSubsystem = IOnlineSubsystem::Get();
	checkf(OnlineSubsystem != nullptr, TEXT("Unable to get the SubSytem"));

	IdentityInterface = OnlineSubsystem->GetIdentityInterface();
	checkf(IdentityInterface != nullptr, TEXT("Unable to get the Identity interface"));

	LoginCompleteDelegateHandle = IdentityInterface->AddOnLoginCompleteDelegate_Handle(0, LoginCompleteDelegate);
	LogoutCompleteDelegateHandle = IdentityInterface->AddOnLogoutCompleteDelegate_Handle(0, LogoutCompleteDelegate);
	LoginChangedDelegateHandle = IdentityInterface->AddOnLoginChangedDelegate_Handle(LoginChangedDelegate);
}

void UOTIdentitySubsystem::Deinitialize()
{
	Super::Deinitialize();

	IdentityInterface->ClearOnLoginCompleteDelegate_Handle(0, LoginCompleteDelegateHandle);
	IdentityInterface->ClearOnLogoutCompleteDelegate_Handle(0, LogoutCompleteDelegateHandle);
	IdentityInterface->ClearOnLoginChangedDelegate_Handle(LoginChangedDelegateHandle);
}

void UOTIdentitySubsystem::Login(FString MyId, FString MyToken, FString MyType)
{
	checkf(IdentityInterface != nullptr, TEXT("Unable to get the Identity interface"));

	FOnlineAccountCredentials OnlineAccountCredentials;
	OnlineAccountCredentials.Id = FString(MyId);
	OnlineAccountCredentials.Token = FString(MyToken);
	OnlineAccountCredentials.Type = FString(MyType);
	
	IdentityInterface->Login(0,OnlineAccountCredentials);
}

CSteamID UOTIdentitySubsystem::GetLocalSteamID() const
{
	return SteamUser()->GetSteamID();
}

CSteamID UOTIdentitySubsystem::GetSteamId(const FOTUniqueNetId& UniqueNetId) const
{
	return CSteamID(*((uint64*)UniqueNetId.NativeUniqueNetId->GetBytes()));
}
UTexture2D* UOTIdentitySubsystem::GetSteamAvatar(const FOTUniqueNetId& UniqueNetId, const EOTAvatarSize AvatarSize) const
{
	
	int32 Avatar = 0;
	uint32 Width = 0;
	uint32 Height = 0;
	CSteamID id = GetSteamId(UniqueNetId);
	if (SteamAPI_Init())
	{
		switch (AvatarSize)
		{
		case EOTAvatarSize::Small:
			Avatar = SteamFriends()->GetSmallFriendAvatar(id);
			break;
		case EOTAvatarSize::Medium:
			Avatar = SteamFriends()->GetMediumFriendAvatar(id);
			break;
		case EOTAvatarSize::Large:
			Avatar = SteamFriends()->GetLargeFriendAvatar(id);
			break;
		}

		if (Avatar == -1)
			return nullptr;


		SteamUtils()->GetImageSize(Avatar, &Width, &Height);
		
		if (Width > 0 && Height > 0)
		{
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			uint8 *oAvatarRGBA = new uint8[Width * Height * 4];


			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Avatar, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));


			

			UTexture2D* AvatarTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);

			uint8* MipData = (uint8*)AvatarTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			AvatarTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

			// Original implementation was missing this!!
			// the hell man......
			delete[] oAvatarRGBA;

			//Setting some Parameters for the Texture and finally returning it
			AvatarTexture->GetPlatformData()->SetNumSlices(1);
			AvatarTexture->NeverStream = true;
			//Avatar->CompressionSettings = TC_EditorIcon;

			AvatarTexture->UpdateResource();

			return AvatarTexture;
		}
	}
	return nullptr;
}

FOTUniqueNetId UOTIdentitySubsystem::GetLocalUniqueNetId() const
{
	return FOTUniqueNetId(IdentityInterface->GetUniquePlayerId(0));
}
void UOTIdentitySubsystem::AutoLogin()
{
	IdentityInterface->AutoLogin(0);
}

void UOTIdentitySubsystem::Logout()
{
	checkf(IdentityInterface != nullptr, TEXT("Unable to get the Identity interface"));
	IdentityInterface->Logout(0);
}

bool UOTIdentitySubsystem::IsLocalPlayerLoggedIn() const
{
	
	checkf(IdentityInterface != nullptr, TEXT("Unable to get the Identity interface"));

	return IdentityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}

FString UOTIdentitySubsystem::GetPlayerNickname() const
{
	checkf(IdentityInterface != nullptr, TEXT("Unable to get the Identity interface"));
	if(IsLocalPlayerLoggedIn())
		return IdentityInterface->GetPlayerNickname(0);
	else
		return FString("Not Logged In");
}

void UOTIdentitySubsystem::HandleLoginComplete(int LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
                                               const FString& Error) const
{
	OnLoginComplete.Broadcast(bWasSuccessful, UserId.ToString());
}

void UOTIdentitySubsystem::HandleLogoutComplete(int LocalUserNum, bool bWasSuccessful)
{
	OnLogoutComplete.Broadcast(bWasSuccessful);
}

void UOTIdentitySubsystem::HandleLoginChange(int LocalUserNum)
{
	OnLoginChanged.Broadcast(true);
}
