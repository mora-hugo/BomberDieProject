// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/OTSessionMenu.h"

#include "Sessions/OTSessionsSubsystem.h"
#include "Sessions/Structs/OTCreateSessionsOptions.h"
#include "Sessions/Structs/OTFindSessionsOptions.h"

void UOTSessionMenu::MenuSetup(bool AddWidgetToWiewport, bool RemoveAtTeardown)
{
	bRemoveAtTearDown = RemoveAtTeardown;
	if(AddWidgetToWiewport)
		AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	if(UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if(UGameInstance* Instance = GetGameInstance())
	{
		OTSessionsSubsystem = Instance->GetSubsystem<UOTSessionsSubsystem>();
		checkf(OTSessionsSubsystem != nullptr, TEXT("Multiplayer Session Subsystem cannot be found"));

		OTSessionsSubsystem->ToolboxOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		OTSessionsSubsystem->ToolboxOnFindSessionComplete.AddDynamic(this, &ThisClass::OnFindSession);
		OTSessionsSubsystem->ToolboxOnJoinSessionComplete.AddDynamic(this, &ThisClass::OnJoinSession);
	}
}

void UOTSessionMenu::HostSession(const TSoftObjectPtr<UWorld> LobbyLevel,const FString& SessionName, int32 NumPublicConnection, const bool bIsLan, const FString& Map, const FString& SessionType)
{
	if(!ensureMsgf(OTSessionsSubsystem != nullptr,
		TEXT("Multiplayer Session Subsystem is not set. Did you call MenuSetup?"))) return;

	//Retrieve the path to the level
	LobbyLevel.ToSoftObjectPath().ToString().Split(FString("."),&LobbyMap, nullptr );
	
	FOTCreateSessionsOptions CreateSessionOptions {};
	CreateSessionOptions.SessionName = SessionName;
	CreateSessionOptions.Map = Map;
	CreateSessionOptions.NumPublicConnections = NumPublicConnection;
	CreateSessionOptions.bIsInLAN = bIsLan;
	CreateSessionOptions.SessionType = SessionType;
	OTSessionsSubsystem->CreateSession(CreateSessionOptions);
}

void UOTSessionMenu::FindSession(bool bIsLan, const FString& SessionName, const FString& Map, const FString& SessionType, const int32 MaxSearchResults)
{
	if(!ensureMsgf(OTSessionsSubsystem != nullptr,
		TEXT("Multiplayer Session Subsystem is not set. Did you call MenuSetup?"))) return;

	FOTFindSessionOptions FindSessionOptions {};
	FindSessionOptions.bIsInLAN = bIsLan;
	FindSessionOptions.SessionName = SessionName;
	FindSessionOptions.Map = Map;
	FindSessionOptions.SessionType = SessionType;
	FindSessionOptions.MaxSearchResults = MaxSearchResults;
	OTSessionsSubsystem->FindSessions(FindSessionOptions);
}

void UOTSessionMenu::JoinSession(const FOTSessionSearchResult& session)
{
	if(!ensureMsgf(OTSessionsSubsystem != nullptr,
		TEXT("Multiplayer Session Subsystem is not set. Did you call MenuSetup?"))) return;

	OTSessionsSubsystem->JoinSession(session);
}



void UOTSessionMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void UOTSessionMenu::OnCreateSession(bool bWasSuccessful)
{
	OnSessionCreateComplete.Broadcast(bWasSuccessful);

	if(!bWasSuccessful) return;
	if(auto* World = GetWorld())
	{
		World->ServerTravel(LobbyMap.Append("?listen"));
	}
}

void UOTSessionMenu::OnFindSession(const TArray<FOTSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	OnSessionSearchComplete.Broadcast(bWasSuccessful, SessionResults);
}

void UOTSessionMenu::OnJoinSession(bool bWasSuccessful, EOTJoinSessionResultType Type, const FString& Address)
{
	OnSessionJoinedComplete.Broadcast(bWasSuccessful, Type);
	
	if(auto* World = GetWorld())
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeGameOnly InputModeData;

			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UOTSessionMenu::MenuTearDown()
{
	
	if(!OTSessionsSubsystem) return;
	OTSessionsSubsystem->ToolboxOnCreateSessionComplete.RemoveDynamic(this, &ThisClass::OnCreateSession);
	OTSessionsSubsystem->ToolboxOnFindSessionComplete.RemoveDynamic(this, &ThisClass::OnFindSession);
	OTSessionsSubsystem->ToolboxOnJoinSessionComplete.RemoveDynamic(this, &ThisClass::OnJoinSession);

	if(bRemoveAtTearDown)
		RemoveFromParent();
}
