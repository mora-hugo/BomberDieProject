// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Friends/OTFriend.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Sessions/OTJoinSessionResultType.h"
#include "Structs/OTCreateSessionsOptions.h"
#include "Structs/OTFindSessionsOptions.h"
#include "Structs/OTSessionSearchResult.h"

#include "OTSessionsSubsystem.generated.h"

//
// Declaring custom delegates
//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToolboxOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FToolboxOnFindSessionComplete, const TArray<FOTSessionSearchResult>&, SessionResults,
																			bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FToolboxOnJoinSessionComplete, bool, bWasSuccessful, EOTJoinSessionResultType, Type, const FString&, Address);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToolboxOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToolboxOnStartSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOTOnSessionUserInviteAccepted, bool, bWasSuccessfull, int, ControllerId, const FString&, UniqueNetId, 
	const FOTSessionSearchResult&, OnlineSessionSearchResult);
/**
 * 
 */
UCLASS()
class ONLINETOOLBOX_API UOTSessionsSubsystem : public UGameInstanceSubsystem
{
private:
	GENERATED_BODY()
public:
	UOTSessionsSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	/**
	 * @brief Create a session for the default online subsystem
	 * @param CreateSessionsOptions Struct with session informations
	 */
	UFUNCTION(BlueprintCallable)
	void CreateSession(FOTCreateSessionsOptions CreateSessionsOptions);
	
	
	/**
	 * @brief Find sessions in the default online subsystem
	 * @param SearchOptions Struct with search filter and MaxSearchResults
	 */
	UFUNCTION(BlueprintCallable)
	void FindSessions(FOTFindSessionOptions SearchOptions);

	/**
	 * @brief Join a session in the default online subsystem
	 * @param SessionResult Struct contains session informations
	 */
	UFUNCTION(BlueprintCallable)
	void JoinSession(const FOTSessionSearchResult& SessionResult);

	/** Destroy the current session
	 * @brief 
	 */
	UFUNCTION(BlueprintCallable)
	void DestroySession();

	/** Start the current session
	 * @brief 
	 */
	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void SendInviteToFriend(const FOTFriend& Friend);

	UFUNCTION(BlueprintCallable)
	bool IsInSession() const;

	const FNamedOnlineSession * GetCurrentSession() const;
	
	//
	// Public delegates for class to bind callbacks
	//
	UPROPERTY(BlueprintAssignable)
	FToolboxOnCreateSessionComplete ToolboxOnCreateSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FToolboxOnDestroySessionComplete ToolboxOnDestroySessionComplete;

	UPROPERTY(BlueprintAssignable)
	FToolboxOnFindSessionComplete ToolboxOnFindSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FToolboxOnJoinSessionComplete ToolboxOnJoinSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FToolboxOnStartSessionComplete ToolboxOnStartSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FOTOnSessionUserInviteAccepted OTOnSessionUserInviteAccepted;

	
protected:
	//
	// Internal callbacks for the delegates.
	//
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnSessionUserInviteAccepted(bool bWasSuccessfull, int ControllerId, TSharedPtr<const FUniqueNetId> UserId,
	const FOnlineSessionSearchResult& OnlineSessionSearchResult);

private:
	IOnlineSessionPtr SessionInterface;

	bool bCreateSessionOnDestroy {false};
	int32 LastNumPublicConnections;
	FString LastMatchType;
	
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	//
	// Delegates for the OnlineSubsystem session interfaces
	//
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateeHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateeHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnSessionUserInviteAcceptedDelegate SessionUserInviteAcceptedDelegate;



};
