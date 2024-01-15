#include "Friends/OTFriendsSubsystem.h"

#include "Friends/OTFriend.h"
#include "Friends/OTBlockedUser.h"

// Unused in EOS

UOTFriendsSubsystem::UOTFriendsSubsystem() : _OnReadFriendListCompleteDelegate(FOnReadFriendsListComplete::CreateUObject(this,&ThisClass::OnReadFriendsListComplete)),
											 _OnQueryBlockedPlayersCompleteDelegate(FOnQueryBlockedPlayersCompleteDelegate::CreateUObject(this,&ThisClass::OnQueryBlockedPlayersComplete)),
											_OnAcceptInviteCompleteDelegate(FOnAcceptInviteComplete::CreateUObject(this,&ThisClass::OnAcceptInviteComplete)),
											_OnInviteReceivedDelegate(FOnInviteReceivedDelegate::CreateUObject(	this, &ThisClass::OnOtherPlayerInviteReceived)),
											 _OnInviteRejectedDelegate(FOnInviteRejectedDelegate::CreateUObject(this, &ThisClass::OnOtherPlayerInviteRejected)),
											 _OnInviteAcceptedDelegate(FOnInviteAcceptedDelegate::CreateUObject(this, &ThisClass::OnOtherPlayerInviteAccepted)),
											 _OnSendInviteCompleteDelegate(FOnSendInviteComplete::CreateUObject(this, &ThisClass::OnSendInviteCompleted)),
											_OnInviteRejectCompleteDelegate(FOnRejectInviteCompleteDelegate::CreateUObject(this,&ThisClass::OnRejectInviteComplete)),
											_OnDeleteFriendFromListDelegate(FOnDeleteFriendCompleteDelegate::CreateUObject(this,&ThisClass::OnFriendRemoveFromList)),
											_OnBlockPlayerDelegate(FOnBlockedPlayerCompleteDelegate::CreateUObject(this,&ThisClass::OnUserBlocked)),
											_OnUnblockPlayerDelegate(FOnUnblockedPlayerCompleteDelegate::CreateUObject(this,&ThisClass::OnUserUnblocked))


											 	
{
	
}

void UOTFriendsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnlineInterface = IOnlineSubsystem::Get();
	if(!ensureMsgf(OnlineInterface != nullptr, TEXT("Unable to get the Online Subsystem"))) return;
	
	
	FriendsInterface = OnlineInterface->GetFriendsInterface();
	if(!ensureMsgf(FriendsInterface != nullptr, TEXT("Unable to get the Friends interface"))) return;
	

	IdentityInterface = OnlineInterface->GetIdentityInterface();
	if(!ensureMsgf(IdentityInterface != nullptr, TEXT("Unable to get the Identity interface"))) return;

	// Initialize notifier delegates
	
	const auto OnInviteReceivedDelegate = FOnInviteReceivedDelegate::CreateUObject(this, &ThisClass::OnOtherPlayerInviteReceived);
	_OnInviteReceivedDelegateHandle = FriendsInterface->AddOnInviteReceivedDelegate_Handle(OnInviteReceivedDelegate);

	const auto OnInviteRejectedDelegate = FOnInviteRejectedDelegate::CreateUObject(this, &ThisClass::OnOtherPlayerInviteRejected);
	_OnInviteRejectedDelegateHandle = FriendsInterface->AddOnInviteRejectedDelegate_Handle(OnInviteRejectedDelegate);

	const auto OnInviteAcceptedDelegate = FOnInviteAcceptedDelegate::CreateUObject(this, &ThisClass::OnOtherPlayerInviteAccepted);
	_OnInviteAcceptedDelegateHandle = FriendsInterface->AddOnInviteAcceptedDelegate_Handle(OnInviteAcceptedDelegate);
}

void UOTFriendsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


void UOTFriendsSubsystem::UpdateFriendList()
{
	const bool bSuccessful = FriendsInterface->ReadFriendsList(
		0,
		FRIEND_LIST_NAME,
		_OnReadFriendListCompleteDelegate
	);

	if (!bSuccessful)
	{
		OTOnFriendListUpdatedDelegate.Broadcast(false, {});
	}
}

void UOTFriendsSubsystem::UpdateBlockedUsers()
{
	// Add the delegate
	_OnQueryBlockedPlayersCompleteDelegateHandle = FriendsInterface->AddOnQueryBlockedPlayersCompleteDelegate_Handle(_OnQueryBlockedPlayersCompleteDelegate);


	const bool bSuccessful = FriendsInterface->QueryBlockedPlayers(
		*IdentityInterface->GetUniquePlayerId(0)
	);
	if (!bSuccessful)
	{
		OTOnBlockedUserUpdatedDelegate.Broadcast(false, {});
		FriendsInterface->ClearOnQueryBlockedPlayersCompleteDelegate_Handle(_OnQueryBlockedPlayersCompleteDelegateHandle);
	}
}

void UOTFriendsSubsystem::GetCachedFriends(TArray<FOTFriend>& Friends) const
{
	Friends.Reset();
	TArray<TSharedRef<FOnlineFriend>> FriendsArr;

	FriendsInterface->GetFriendsList(
		0 /* LocalUserNum */,
		FRIEND_LIST_NAME /* ListName, unused by EOS */,
		FriendsArr /* OutFriends */
	);

	// Create the wrapper class
	for (const auto& Friend : FriendsArr)
	{
		FOTFriend MyFriend;
		MyFriend.User.Name = Friend->GetDisplayName();
		MyFriend.User.UniqueNetId = FOTUniqueNetId(Friend->GetUserId()->AsShared());
		MyFriend.OnlineFriend = Friend;
		Friends.Add(MyFriend);
		
	}
}






void UOTFriendsSubsystem::GetCachedBlockedUsers(TArray<FOTBlockedUser>& UsersBlocked) const
{
	TArray<TSharedRef<FOnlineBlockedPlayer>> BlockedPlayers;
	FriendsInterface->GetBlockedPlayers(
		*IdentityInterface->GetUniquePlayerId(0), BlockedPlayers);

	// Clean the blocked list
	UsersBlocked.Reset();

	// Create the wrapper class
	for (const auto& User : BlockedPlayers)
	{
		FOTBlockedUser UserBlocked;
		UserBlocked.User.Name = User->GetDisplayName();
		UserBlocked.User.UniqueNetId = FOTUniqueNetId(User->GetUserId()->AsShared());
		UserBlocked.OnlineBlockUser = User;
		UsersBlocked.Add(UserBlocked);
	}
}

void UOTFriendsSubsystem::SendInvite(const FOTUser& User)
{
	const bool bSuccessful = FriendsInterface->SendInvite(
		0 /* LocalUserNum */,
		User.UniqueNetId.NativeUniqueNetId->AsShared().Get()/* The user to send the invite to */,
		FRIEND_LIST_NAME /* ListName, unused by EOS */,
		_OnSendInviteCompleteDelegate
	);

	if (!bSuccessful)
	{
		this->OTOnSendInviteCompleteDelegate.Broadcast(false,{});
	}
}

void UOTFriendsSubsystem::AcceptInvite(const FOTUser& User)
{

	const bool bSuccessful = FriendsInterface->AcceptInvite(
		0 /* LocalUserNum */,
		User.UniqueNetId.NativeUniqueNetId->AsShared().Get() /* The user who sent the original invite */,
		FRIEND_LIST_NAME /* ListName, unused by EOS */,
		_OnAcceptInviteCompleteDelegate);
	if (!bSuccessful)
	{
		this->OTOnAcceptInviteDelegate.Broadcast(false, {});
	}
}

void UOTFriendsSubsystem::RejectInvite(const FOTUser& User)
{
	
	_OnInviteRejectCompleteDelegateHandle = FriendsInterface->AddOnRejectInviteCompleteDelegate_Handle(0,_OnInviteRejectCompleteDelegate);
	const bool bSuccessful = FriendsInterface->RejectInvite(
		0 /* LocalUserNum */,
		User.UniqueNetId.NativeUniqueNetId->AsShared().Get() /* The user who sent the original invite */,
		FRIEND_LIST_NAME /* ListName, unused by EOS */
	);
	if (!bSuccessful)
	{
		OTOnAcceptInviteDelegate.Broadcast(false, {});
		FriendsInterface->ClearOnRejectInviteCompleteDelegate_Handle(0,_OnInviteRejectCompleteDelegateHandle);
	}
}

void UOTFriendsSubsystem::RemoveFriendFromList(const FOTFriend& Friend)
{
	
	
	_OnDeleteFriendFromListDelegateHandle = FriendsInterface->AddOnDeleteFriendCompleteDelegate_Handle(0, _OnDeleteFriendFromListDelegate);
	const bool bSuccessful = FriendsInterface->DeleteFriend(
		0 /* LocalUserNum */,
		Friend.User.UniqueNetId.NativeUniqueNetId->AsShared().Get() /* The user who sent the original invite */,
		FRIEND_LIST_NAME /* ListName, unused by EOS */);
	if (!bSuccessful)
	{
		OTOnFriendDeletedDelegate.Broadcast(false, {});
		FriendsInterface->ClearOnDeleteFriendCompleteDelegate_Handle(0,_OnDeleteFriendFromListDelegateHandle);
	}
}

bool UOTFriendsSubsystem::IsFriend(const FOTUser& User) const
{
	return FriendsInterface->IsFriend(
		0,
		User.UniqueNetId.NativeUniqueNetId->AsShared().Get(),
		FRIEND_LIST_NAME);
}

void UOTFriendsSubsystem::BlockUser(const FOTUser& User)
{
	
	_OnBlockPlayerDelegateHandle = FriendsInterface->AddOnBlockedPlayerCompleteDelegate_Handle(0, _OnBlockPlayerDelegate);
	//Use block player function
	const bool bSuccessful = FriendsInterface->BlockPlayer(0, *User.UniqueNetId.NativeUniqueNetId);
	if(!bSuccessful)
	{
		OTOnUserBlockedDelegate.Broadcast(bSuccessful,{});
		FriendsInterface->ClearOnBlockedPlayerCompleteDelegate_Handle(0,_OnBlockPlayerDelegateHandle);
	}
}

void UOTFriendsSubsystem::UnBlockUser(const FOTUser& User)
{
	
	_OnUnblockPlayerDelegateHandle = FriendsInterface->AddOnUnblockedPlayerCompleteDelegate_Handle(0, _OnUnblockPlayerDelegate);
	const bool bSuccessful = FriendsInterface->UnblockPlayer(0, *User.UniqueNetId.NativeUniqueNetId);
	if(!bSuccessful)
	{
		OTOnUserUnblockedDelegate.Broadcast(bSuccessful,{});
		FriendsInterface->ClearOnUnblockedPlayerCompleteDelegate_Handle(0,_OnUnblockPlayerDelegateHandle);
	}
}


/*  Response functions */


void UOTFriendsSubsystem::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName,
                                                    const FString& ErrorStr)
{
	TArray<FOTFriend> MyFriendsList;
	GetCachedFriends(MyFriendsList);
	OTOnFriendListUpdatedDelegate.Broadcast(bWasSuccessful, MyFriendsList);
}

void UOTFriendsSubsystem::OnQueryBlockedPlayersComplete(const FUniqueNetId& UserId, bool bWasSuccessful,
	const FString& Error)
{
	FriendsInterface->ClearOnQueryBlockedPlayersCompleteDelegate_Handle(_OnQueryBlockedPlayersCompleteDelegateHandle);
	TArray<FOTBlockedUser> BlockedUsers;
	if(bWasSuccessful)
	{
		//Getting cached blocked users
		GetCachedBlockedUsers(BlockedUsers);
	}
	
	OTOnBlockedUserUpdatedDelegate.Broadcast(bWasSuccessful,BlockedUsers);
	
	
}

void UOTFriendsSubsystem::OnRejectInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId,
												 const FString& ListName, const FString& ErrorStr)
{
	FriendsInterface->ClearOnRejectInviteCompleteDelegate_Handle(0,_OnInviteRejectCompleteDelegateHandle);
	if (!bWasSuccessful)
		return OTOnRejectInviteDelegate.Broadcast(false, {});
	FOTUser User;
	User.UniqueNetId = FOTUniqueNetId(FriendId.AsShared());
	OTOnRejectInviteDelegate.Broadcast(bWasSuccessful, User);
}
void UOTFriendsSubsystem::OnSendInviteCompleted(int32 LocalUserNum, bool bWasSuccessful,
                                                const FUniqueNetId& FriendId, const FString& ListName,
                                                const FString& ErrorStr)
{
	
	if (!bWasSuccessful)
		return OTOnSendInviteCompleteDelegate.Broadcast(false, {});
	
	FOTUser User;
	User.UniqueNetId = FOTUniqueNetId(FriendId.AsShared());
	OTOnSendInviteCompleteDelegate.Broadcast(bWasSuccessful, User);
}

void UOTFriendsSubsystem::OnOtherPlayerInviteReceived(const FUniqueNetId& UserId, const FUniqueNetId& FriendId)
{
	const TSharedPtr<FOnlineFriend> Friend = FriendsInterface->GetFriend(0, FriendId, FRIEND_LIST_NAME);

	FOTFriend MyFriend;
	if (!Friend.IsValid())
		return OTOnInviteReceiveDelegate.Broadcast(false, MyFriend);
	MyFriend.User.Name = Friend->GetDisplayName();
	MyFriend.User.UniqueNetId = FOTUniqueNetId(Friend->GetUserId()->AsShared());
	MyFriend.OnlineFriend = Friend;
	

	
	OTOnInviteReceiveDelegate.Broadcast(true, MyFriend);
}


void UOTFriendsSubsystem::OnAcceptInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId,
                                                 const FString& ListName, const FString& ErrorStr)
{
	const TSharedPtr<FOnlineFriend> Friend = FriendsInterface->GetFriend(LocalUserNum, FriendId, FRIEND_LIST_NAME);
	FOTFriend MyFriend;
	if (!Friend.IsValid())
		return OTOnAcceptInviteDelegate.Broadcast(false, MyFriend);
	MyFriend.User.Name = Friend->GetDisplayName();
	MyFriend.User.UniqueNetId = FOTUniqueNetId(Friend->GetUserId()->AsShared());
	MyFriend.OnlineFriend = Friend;
	OTOnAcceptInviteDelegate.Broadcast(bWasSuccessful, MyFriend);
}



void UOTFriendsSubsystem::OnOtherPlayerInviteAccepted(const FUniqueNetId& UserId, const FUniqueNetId& FriendId)
{
	const TSharedPtr<FOnlineFriend> Friend = FriendsInterface->GetFriend(0, FriendId, FRIEND_LIST_NAME);
	FOTFriend MyFriend;
	if (!Friend.IsValid())
		return OTOnInviteAcceptDelegate.Broadcast(false, MyFriend);
	MyFriend.User.Name = Friend->GetDisplayName();
	MyFriend.User.UniqueNetId = FOTUniqueNetId(Friend->GetUserId()->AsShared());
	MyFriend.OnlineFriend = Friend;
	
	OTOnInviteAcceptDelegate.Broadcast(true, MyFriend);
}

void UOTFriendsSubsystem::OnOtherPlayerInviteRejected(const FUniqueNetId& UserId, const FUniqueNetId& FriendId)
{
	
	FOTUser User;
	User.UniqueNetId = FOTUniqueNetId(FriendId.AsShared());
	OTOnInviteRejectDelegate.Broadcast(true, User);
}

void UOTFriendsSubsystem::OnFriendRemoveFromList(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId,
                                                 const FString& ListName, const FString& ErrorStr)
{
; 	FriendsInterface->ClearOnDeleteFriendCompleteDelegate_Handle(0,_OnDeleteFriendFromListDelegateHandle);
	if(!bWasSuccessful)
		return OTOnFriendDeletedDelegate.Broadcast(bWasSuccessful, {});

	FOTUser User;
	User.UniqueNetId = FOTUniqueNetId(FriendId.AsShared());
	OTOnFriendDeletedDelegate.Broadcast(true, User);
}

void UOTFriendsSubsystem::OnUserBlocked(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UniqueID,
                                        const FString& ListName, const FString& ErrorStr)
{
	FriendsInterface->ClearOnBlockedPlayerCompleteDelegate_Handle(0,_OnBlockPlayerDelegateHandle);
	
	FOTBlockedUser User;
	if (!bWasSuccessful)
		return OTOnUserBlockedDelegate.Broadcast(bWasSuccessful, {});
	User.User.UniqueNetId = FOTUniqueNetId(UniqueID.AsShared());
	OTOnUserBlockedDelegate.Broadcast(bWasSuccessful, User);
}

void UOTFriendsSubsystem::OnUserUnblocked(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UniqueID,
                                          const FString& ListName, const FString& ErrorStr)
{
	FriendsInterface->ClearOnUnblockedPlayerCompleteDelegate_Handle(0,_OnUnblockPlayerDelegateHandle);
	FOTBlockedUser User;
	if (!bWasSuccessful)
		return OTOnUserUnblockedDelegate.Broadcast(bWasSuccessful, {});
	User.User.UniqueNetId = FOTUniqueNetId(UniqueID.AsShared());
	OTOnUserBlockedDelegate.Broadcast(bWasSuccessful, User);
}