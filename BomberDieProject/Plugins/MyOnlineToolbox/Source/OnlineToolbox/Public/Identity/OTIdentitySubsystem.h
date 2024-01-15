// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Friends/OTUniqueNetId.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Steamworks/Steamv153/sdk/public/steam/steam_api.h"
#include "Steamworks/Steamv153/sdk/public/steam/isteamuser.h"
#include "Steamworks/Steamv153/sdk/public/steam/isteamutils.h"
#include "OTIdentitySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTLoginCompleteDelegate, bool, bWasSuccessful, const FString, UserId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOTLogoutCompleteDelegate, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOTLoginChangeDelegate, bool, bWasSuccessful);

UENUM(BlueprintType)
enum class EOTAvatarSize : uint8
{
	Small,
	Medium,
	Large
};


UCLASS()
class ONLINETOOLBOX_API UOTIdentitySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	explicit UOTIdentitySubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category="Identity")
	void Login(FString MyId, FString MyToken, FString MyType);

	CSteamID GetLocalSteamID() const;

	CSteamID GetSteamId(const FOTUniqueNetId& UniqueNetId) const;

	UFUNCTION(BlueprintCallable)
	UTexture2D * GetSteamAvatar(const FOTUniqueNetId& UniqueNetId, const EOTAvatarSize AvatarSize) const;
	
	UFUNCTION(BlueprintCallable, Category="Identity")
	void AutoLogin();
	
	UFUNCTION(BlueprintCallable, Category="Identity")
	void Logout();

	UFUNCTION(BlueprintCallable, Category="Identity")
	bool IsLocalPlayerLoggedIn() const;


	UFUNCTION(BlueprintCallable, Category="Identity")
	FOTUniqueNetId GetLocalUniqueNetId() const;

	UFUNCTION(BlueprintCallable, Category="Identity")
	FString GetPlayerNickname() const;

	UPROPERTY(BlueprintAssignable, Category="Identity")
	FOTLoginCompleteDelegate OnLoginComplete;

	UPROPERTY(BlueprintAssignable, Category="Identity")
	FOTLogoutCompleteDelegate OnLogoutComplete;

	UPROPERTY(BlueprintAssignable, Category="Identity")
	FOTLoginChangeDelegate OnLoginChanged;

private:
	IOnlineSubsystem * OnlineSubsystem;
	IOnlineIdentityPtr IdentityInterface;

	FDelegateHandle LoginCompleteDelegateHandle;
	FDelegateHandle LogoutCompleteDelegateHandle;
	FDelegateHandle LoginChangedDelegateHandle;

	FOnLoginCompleteDelegate LoginCompleteDelegate;
	FOnLogoutCompleteDelegate LogoutCompleteDelegate;
	FOnLoginChangedDelegate LoginChangedDelegate;

	void HandleLoginComplete(int LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) const;
	void HandleLogoutComplete(int LocalUserNum, bool bWasSuccessful);
	void HandleLoginChange(int LocalUserNum);
};
