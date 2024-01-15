// Fill out your copyright notice in the Description page of Project Settings.
#include "Sessions/OTSessionsSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Sessions/Structs/OTCreateSessionsOptions.h"
#include "Sessions/Structs/OTFindSessionsOptions.h"
#include "Sessions/Structs/OTSessionSearchResult.h"


UOTSessionsSubsystem::UOTSessionsSubsystem() :
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UOTSessionsSubsystem::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UOTSessionsSubsystem::OnFindSessionComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UOTSessionsSubsystem::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UOTSessionsSubsystem::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &UOTSessionsSubsystem::OnStartSessionComplete)),
	SessionUserInviteAcceptedDelegate(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UOTSessionsSubsystem::OnSessionUserInviteAccepted))
{
	
}

void UOTSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// print the current subsystem
	UE_LOG(LogTemp, Warning, TEXT("Current Subsystem: %s"), *IOnlineSubsystem::Get()->GetSubsystemName().ToString());
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem != nullptr, TEXT("Unable to get the SubSytem"));

	SessionInterface = Subsystem->GetSessionInterface();
	if(!ensureMsgf(SessionInterface.IsValid(), TEXT("Unable to get the Session Interface"))) return;

	
	SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(SessionUserInviteAcceptedDelegate);
	SessionInterface->AddOnSessionInviteReceivedDelegate_Handle(FOnSessionInviteReceivedDelegate::CreateLambda([=](const FUniqueNetId& /*UserId*/, const FUniqueNetId& /*FromId*/, const FString& /*AppId*/, const FOnlineSessionSearchResult& /*InviteResult*/)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Invite Received"));
	}));
	UE_LOG(LogTemp, Warning, TEXT("Delegates created"));

}

void UOTSessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UOTSessionsSubsystem::CreateSession(FOTCreateSessionsOptions CreateSessionsOptions)
{
	if(!ensureMsgf(SessionInterface.IsValid(), TEXT("Unable to get the Session Interface"))) return;
	//Destroy existing session if it exists
	const auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(ExistingSession != nullptr)
	{
		DestroySession();
		return;
	}

	//Register the delegate for when the creation complete and store its handle for later removal
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	//Create the session parameters. We could not use the MakeShareable and thus the new
	//but this way we can make SessionSettings a member of our class for future reuse.
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	
	//If we are using the NULL subsystem it is a LAN match. Otherwise it is an online match
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = CreateSessionsOptions.NumPublicConnections;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->Set(FName("SessionType"), CreateSessionsOptions.SessionType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("SessionName"), CreateSessionsOptions.SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("Map"), CreateSessionsOptions.Map, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


#if !UE_BUILD_SHIPPING
	//Enforce a specific Build ID in not shipping so we can
	//easily test session creation²
	LastSessionSettings->BuildUniqueId = 1;
#endif

	//Create the session
	const bool success = SessionInterface->CreateSession(0, NAME_GameSession, *LastSessionSettings);
	if(!success)
	{

		//We failed to create the session simply remove the delegate for completion
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		ToolboxOnCreateSessionComplete.Broadcast(false);
	}
	
}



void UOTSessionsSubsystem::FindSessions(FOTFindSessionOptions SearchOptions)
{
	if(!ensureMsgf(SessionInterface.IsValid(), TEXT("Unable to get the Session Interface"))) return;
	//Register the delegate for when the find session complete and store its handle for later removal
	FindSessionsCompleteDelegateeHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = SearchOptions.MaxSearchResults;

	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	//Needed to search session
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


	//Each parameter is set only if the FString is not empty
	if(!SearchOptions.SessionType.IsEmpty())
		LastSessionSearch->QuerySettings.Set(FName("SessionType"), SearchOptions.SessionType, EOnlineComparisonOp::Equals);
	if(!SearchOptions.Map.IsEmpty())
		LastSessionSearch->QuerySettings.Set(FName("Map"), SearchOptions.Map, EOnlineComparisonOp::Equals);
	if(!SearchOptions.SessionName.IsEmpty())
		LastSessionSearch->QuerySettings.Set(FName("SessionName"), SearchOptions.SessionName, EOnlineComparisonOp::Equals);

	
	
	const bool success = SessionInterface->FindSessions(0, LastSessionSearch.ToSharedRef());	
	if(!success)
	{

		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateeHandle);
		const TArray<FOTSessionSearchResult> Results;
		ToolboxOnFindSessionComplete.Broadcast(Results, false);
	}

}

void UOTSessionsSubsystem::JoinSession(const FOTSessionSearchResult& SessionResult)
{
	if(!ensureMsgf(SessionInterface.IsValid(), TEXT("Unable to get the Session Interface"))) return;

	//Register the delegate for when the join session complete and store its handle for later removal
	JoinSessionCompleteDelegateeHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	
	//Get the local player because create session need the creator Unique Net ID
	const bool success = SessionInterface->JoinSession(0, NAME_GameSession, SessionResult.Native);
	if(!success)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateeHandle);
		ToolboxOnJoinSessionComplete.Broadcast(false, EOTJoinSessionResultType::UnknownError, "");
	}
}

void UOTSessionsSubsystem::DestroySession()
{
	if(!ensureMsgf(SessionInterface.IsValid(), TEXT("Unable to get the Session Interface"))) return;

	//Register the delegate for when the destroy session complete and store its handle for later removal
	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
	
	//Destroy existing session if it exists
	const auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
	else //No session found
	{
		//Remove the destroy session completion delegate
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		
		ToolboxOnDestroySessionComplete.Broadcast(false);
	}
}

void UOTSessionsSubsystem::StartSession()
{
	if(!ensureMsgf(SessionInterface.IsValid(), TEXT("Unable to get the Session Interface"))) return;

	//Register the delegate for when the join session complete and store its handle for later removal
	StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);
	
	if(!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
		ToolboxOnStartSessionComplete.Broadcast(false);
	}

}

void UOTSessionsSubsystem::SendInviteToFriend(const FOTFriend& Friend)
{
	if(Friend.User.UniqueNetId.NativeUniqueNetId.IsValid())
		SessionInterface->SendSessionInviteToFriend(0, NAME_GameSession, Friend.User.UniqueNetId.NativeUniqueNetId->AsShared().Get());

}

bool UOTSessionsSubsystem::IsInSession() const
{
	return GetCurrentSession() != nullptr;
}

const FNamedOnlineSession * UOTSessionsSubsystem::GetCurrentSession() const
{
	if(!SessionInterface) return nullptr;
	return SessionInterface->GetNamedSession(NAME_GameSession);
}

void UOTSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface)
	{
		//Remove the create session completion delegate
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	//Fire the delegate
	ToolboxOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UOTSessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if(SessionInterface)
	{
		//Remove the join session completion delegate
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateeHandle);
	}

	if(!LastSessionSearch.IsValid() || LastSessionSearch->SearchResults.Num() <= 0)
	{
		const TArray<FOTSessionSearchResult> Results;
		ToolboxOnFindSessionComplete.Broadcast(Results, false);
		return;
	}

	TArray<FOTSessionSearchResult> Results;
	Results.Reserve(LastSessionSearch->SearchResults.Num());
	for(const auto& result : LastSessionSearch->SearchResults)
	{
		//Creating our struct to use it in Blueprint
		FOTSessionSearchResult r {};
		r.Native = result;
		r.Session = result.Session;
		r.PingInMs = result.PingInMs;
		r.bIsInLan = result.Session.SessionSettings.bIsLANMatch;
		r.HostName = result.Session.OwningUserName;
		
		
		//Retrieving info from SessionSettings

		r.MaxPublicConnection = result.Session.SessionSettings.NumPublicConnections;
		r.CurrentPublicConnection = result.Session.SessionSettings.NumPublicConnections - result.Session.NumOpenPublicConnections + 1;
		
		result.Session.SessionSettings.Get(FName("Map"),r.Map);
		result.Session.SessionSettings.Get(FName("SessionType"),r.SessionType);
		result.Session.SessionSettings.Get(FName("SessionName"),r.SessionName);

		Results.Add(r);
	}

	ToolboxOnFindSessionComplete.Broadcast(Results, true);
}

void UOTSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(SessionInterface)
	{
		//Remove the join session completion delegate
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateeHandle);
	}
	else
	{
		//SessionInterface is null it is not supported
		ToolboxOnJoinSessionComplete.Broadcast(Result == EOnJoinSessionCompleteResult::Type::Success, UnknownError, "");
		return;
	}
	//Get own error enum corresponding to Online Subsystem errors
	EOTJoinSessionResultType ResultBP = EOTJoinSessionResultType::UnknownError;
	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
		ResultBP = Success;
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		ResultBP = SessionIsFull;
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		ResultBP = SessionDoesNotExist;
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		ResultBP = CouldNotRetrieveAddress;
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		ResultBP = AlreadyInSession;
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		ResultBP = UnknownError;
		break;
	}

	//Get the session address to make a client travel
	FString Address;
	SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

	//Fire our own delegate
	ToolboxOnJoinSessionComplete.Broadcast(Result == EOnJoinSessionCompleteResult::Type::Success, ResultBP, Address);
}

void UOTSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface)
	{
		//Remove the destroy session completion delegate
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	//Fire our own delegate
	ToolboxOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UOTSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface)
	{
		//Remove the create session completion delegate
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}
	
	ToolboxOnStartSessionComplete.Broadcast(bWasSuccessful);
}

void UOTSessionsSubsystem::OnSessionUserInviteAccepted(bool bWasSuccessfull, int ControllerId, TSharedPtr<const FUniqueNetId> UserId,
	const FOnlineSessionSearchResult& OnlineSessionSearchResult)
{
	
	if(!LastSessionSearch.IsValid() || LastSessionSearch->SearchResults.Num() <= 0)
	{
		const TArray<FOTSessionSearchResult> Results;
		ToolboxOnFindSessionComplete.Broadcast(Results, false);
		return;
	}
	//Creating our struct to use it in Blueprint
	FOTSessionSearchResult r {};
	r.Native = OnlineSessionSearchResult;
	r.Session = OnlineSessionSearchResult.Session;
	r.PingInMs = OnlineSessionSearchResult.PingInMs;
	r.bIsInLan = OnlineSessionSearchResult.Session.SessionSettings.bIsLANMatch;
	r.HostName = OnlineSessionSearchResult.Session.OwningUserName;
	
	
	//Retrieving info from SessionSettings

	r.MaxPublicConnection = OnlineSessionSearchResult.Session.SessionSettings.NumPublicConnections;
	r.CurrentPublicConnection = OnlineSessionSearchResult.Session.SessionSettings.NumPublicConnections - OnlineSessionSearchResult.Session.NumOpenPublicConnections + 1;

	OTOnSessionUserInviteAccepted.Broadcast(bWasSuccessfull, ControllerId, UserId->ToString(), r);
	//print User Accepeted !!!
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("User Accepted")));
}
