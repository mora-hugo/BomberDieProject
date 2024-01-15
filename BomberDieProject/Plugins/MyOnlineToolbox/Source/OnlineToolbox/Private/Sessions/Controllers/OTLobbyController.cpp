// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/Controllers/OTLobbyController.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Sessions/OTSessionsSubsystem.h"
#include "Sessions/GameModes/OTLobbyGamemode.h"

void AOTLobbyController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,bIsReady);
	DOREPLIFETIME(ThisClass,PlayersName);
	DOREPLIFETIME(ThisClass,PlayersReady);

}

void AOTLobbyController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOTLobbyController::OnNetCleanup(UNetConnection* Connection)
{
	const auto * GameInstance = GetGameInstance();
	checkf(GameInstance,TEXT("Can't retrieve the game instance"));

	auto * SessionSubsystem = GameInstance->GetSubsystem<UOTSessionsSubsystem>();
	checkf(SessionSubsystem,TEXT("Can't retrieve the session subsystem"));

	//Destroy the session before quit 
	SessionSubsystem->DestroySession(); 

	Super::OnNetCleanup(Connection);
}

void AOTLobbyController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AOTLobbyController::OnRep_IsReady(bool OldReady)
{
	OnReadyModified();
}

void AOTLobbyController::OnRep_PlayerList(TArray<FString> OldList)
{
	OnPlayerListModified();
}

void AOTLobbyController::OnRep_ReadyList(TArray<bool> OldReady)
{
	OnPlayerListModified();
}


void AOTLobbyController::OnPlayerListModified_Implementation()
{
	PlayersState.Empty();
	for(int i = 0; i < PlayersName.Num(); ++i)
	{
		PlayersState.Add(PlayersName[i],PlayersReady[i]);
	}
}

void AOTLobbyController::UpdatePlayerReady(bool bIsPlayerReady)
{
	ServerUpdatePlayerReady(bIsPlayerReady);
}



void AOTLobbyController::ServerUpdatePlayerReady_Implementation(bool bIsPlayerReady)
{
	
	if(!HasAuthority()) return;

	
	//Only work if we have the authority
	auto * GameMode = GetWorld()->GetAuthGameMode();
	if(!GameMode) return;

	AOTLobbyGamemode * LobbyGameMode = Cast<AOTLobbyGamemode>(GameMode);
	if(!LobbyGameMode) return;

	LobbyGameMode->SetPlayerReady(this,bIsPlayerReady);

}
