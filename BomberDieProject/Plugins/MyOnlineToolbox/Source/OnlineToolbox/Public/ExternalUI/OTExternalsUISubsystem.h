// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Friends/OTUniqueNetId.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "Interfaces/OnlineExternalUIInterface.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "OTExternalsUISubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOTOnProfileUIClosedDelegate);
/**
 * 
 */
UCLASS()
class ONLINETOOLBOX_API UOTExternalsUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UOTExternalsUISubsystem();
	
	void Initialize(FSubsystemCollectionBase& Collection) override;
public:
	UFUNCTION(BlueprintCallable)
	void ShowProfileUI(const FOTUniqueNetId& UniqueNetId);

	UFUNCTION(BlueprintCallable)
	void ShowFriendUI(const FOTUniqueNetId& UniqueNetId);

	UFUNCTION(BlueprintCallable)
	void ShowInviteUI(const FOTUniqueNetId& UniqueNetId);
private:
	UFUNCTION()
	void HandleProfileUIClosed();
public:
	UPROPERTY(BlueprintAssignable)
	FOTOnProfileUIClosedDelegate OnOTProfileUIClosedDelegate;
private:

	FOnProfileUIClosedDelegate OnProfileClosedDelegate;
	
	IOnlineSubsystem * Subsystem = nullptr;

	IOnlineExternalUIPtr ExternalUIInterface;
};

