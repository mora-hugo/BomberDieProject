// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Friends/OTUser.h"
#include "OTFriendsSubsystem.generated.h"

struct FOTBlockedUser;
struct FOTFriend;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnFriendListUpdated, bool, bWasSuccessfull, const TArray<FOTFriend>&, Friends);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnSendInvite, bool, bWasSuccessfull, const FOTUser &, User);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnInviteReceived, bool, bWasSuccessfull, const FOTFriend &, Friend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnAcceptInvite, bool, bWasSuccessfull, const FOTFriend &, Friend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnRejectInvite, bool, bWasSuccessfull, const FOTUser &, User);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnInviteAccepted, bool, bWasSuccessfull, const FOTFriend &, Friend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnInviteRejected, bool, bWasSuccessfull, const FOTUser &, User);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnFriendDeleted, bool, bWasSuccessfull, const FOTUser &, User);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnUserBlocked, bool, bWasSuccessfull, const FOTBlockedUser &, Friend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnUserUnblocked, bool, bWasSuccessfull, const FOTUser &, Friend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOTOnBlockedUserUpdated, bool, bWasSuccessfull, const TArray<FOTBlockedUser>&, BlockedUsers);





/**
 * 
 */
UCLASS()
class ONLINETOOLBOX_API UOTFriendsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UOTFriendsSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void UpdateFriendList();

	UFUNCTION(BlueprintCallable)
	void UpdateBlockedUsers();
	
	UFUNCTION(BlueprintCallable)
	void GetCachedFriends(TArray<FOTFriend> & Friends) const;

	UFUNCTION(BlueprintCallable)
	void GetCachedBlockedUsers(TArray<FOTBlockedUser> & UsersBlocked) const;
	
	UFUNCTION(BlueprintCallable)
	void SendInvite(const FOTUser & User);

	UFUNCTION(BlueprintCallable)
	void AcceptInvite(const FOTUser & User);

	UFUNCTION(BlueprintCallable)
	void RejectInvite(const FOTUser & User);

	UFUNCTION(BlueprintCallable)
	void RemoveFriendFromList(const FOTFriend & Friend);
	
	UFUNCTION(BlueprintCallable)
	bool IsFriend(const FOTUser & User) const;

	UFUNCTION(BlueprintCallable)
	void BlockUser(const FOTUser & User);

	UFUNCTION(BlueprintCallable)
	void UnBlockUser(const FOTUser & User);

	
	
	/* Own Delegate */
	UPROPERTY(BlueprintAssignable)
	FOTOnFriendListUpdated OTOnFriendListUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnSendInvite OTOnSendInviteCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnInviteReceived OTOnInviteReceiveDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnAcceptInvite OTOnAcceptInviteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnRejectInvite OTOnRejectInviteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnInviteAccepted OTOnInviteAcceptDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnInviteRejected OTOnInviteRejectDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOTOnFriendDeleted OTOnFriendDeletedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOTOnUserBlocked OTOnUserBlockedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOTOnUserUnblocked OTOnUserUnblockedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOTOnBlockedUserUpdated OTOnBlockedUserUpdatedDelegate;

	
	
private:

	/*  Responses functions */
	void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	void OnQueryBlockedPlayersComplete(const FUniqueNetId &UserId, bool bWasSuccessful, const FString &Error);

	void OnRejectInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);
	
	void OnSendInviteCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);

	void OnAcceptInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);

	void OnOtherPlayerInviteReceived(const FUniqueNetId& UserId, const FUniqueNetId& FriendId);
	
	void OnOtherPlayerInviteAccepted(const FUniqueNetId& UserId, const FUniqueNetId& FriendId);

	void OnOtherPlayerInviteRejected(const FUniqueNetId& UserId, const FUniqueNetId& FriendId);

	void OnFriendRemoveFromList(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);

	void OnUserBlocked(int32 LocalUserNum,bool bWasSuccessful,const FUniqueNetId &UniqueID,const FString &ListName,const FString &ErrorStr);

	void OnUserUnblocked(int32 LocalUserNum,bool bWasSuccessful,const FUniqueNetId &UniqueID,const FString &ListName,const FString &ErrorStr);
	
	
	/* Interfaces */
	IOnlineIdentityPtr IdentityInterface;
	IOnlineSubsystem * OnlineInterface = nullptr;
	IOnlineFriendsPtr FriendsInterface;

	// Friends array - 
	TArray<FOTFriend> LastUpdatedFriends;

	

	


	/* Delegate handle */

	FOnReadFriendsListComplete _OnReadFriendListCompleteDelegate;

	FOnAcceptInviteComplete _OnAcceptInviteCompleteDelegate;
	
	FDelegateHandle _OnInviteReceivedDelegateHandle;
	FOnInviteReceivedDelegate _OnInviteReceivedDelegate;
	
	FDelegateHandle _OnInviteRejectedDelegateHandle;
	FOnInviteRejectedDelegate _OnInviteRejectedDelegate;
	
	FDelegateHandle _OnInviteAcceptedDelegateHandle;
	FOnInviteAcceptedDelegate _OnInviteAcceptedDelegate;
	
	FDelegateHandle _OnQueryBlockedPlayersCompleteDelegateHandle;
	FOnQueryBlockedPlayersCompleteDelegate _OnQueryBlockedPlayersCompleteDelegate;
	
	FDelegateHandle _OnInviteRejectCompleteDelegateHandle;
	FOnRejectInviteCompleteDelegate _OnInviteRejectCompleteDelegate;
	
	FDelegateHandle _OnDeleteFriendFromListDelegateHandle;
	FOnDeleteFriendCompleteDelegate _OnDeleteFriendFromListDelegate;
	
	FDelegateHandle _OnBlockPlayerDelegateHandle;
	FOnBlockedPlayerCompleteDelegate _OnBlockPlayerDelegate;

	FDelegateHandle _OnUnblockPlayerDelegateHandle;
	FOnUnblockedPlayerCompleteDelegate _OnUnblockPlayerDelegate;

	FOnSendInviteComplete _OnSendInviteCompleteDelegate;

	

	
	// Not used in EOS
	inline static const FString FRIEND_LIST_NAME = "";
	
};

