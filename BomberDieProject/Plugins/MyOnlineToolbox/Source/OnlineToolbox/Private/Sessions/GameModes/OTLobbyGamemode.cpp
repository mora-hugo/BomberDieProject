// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/GameModes/OTLobbyGamemode.h"

#include "GameFramework/PlayerState.h"
#include "Sessions/OTSessionsSubsystem.h"
#include "Sessions/Controllers/OTLobbyController.h"



void AOTLobbyGamemode::OnPostLogin(AController* NewController)
{
	//To initialize ready array at start
	if(NewController->HasAuthority())
	{
		InitReadyArray();
	}
	Super::OnPostLogin(NewController);
	
	auto * NewPlayerController = StaticCast<APlayerController*>(NewController);
	if(!ensureMsgf(NewPlayerController,TEXT("Can't Cast AController to APlayerController"))) return;

	const int32 Index = PlayersConnected.Add(NewPlayerController);
	IsReady[Index] = false;
	
	//Updates player controllers array
	UpdateControllersArray();

	
}

void AOTLobbyGamemode::Logout(AController* ExitingController)
{
	auto * NewPlayerController = StaticCast<APlayerController*>(ExitingController);
	if(!ensureMsgf(NewPlayerController,TEXT("Can't Cast AController to APlayerController"))) return;

	const auto Index = PlayersConnected.Find(NewPlayerController);
	
	const bool bRemoved = (PlayersConnected.Remove(NewPlayerController) != 0);
	checkf(bRemoved,TEXT("The Player controller has not been removed at logout"));
	IsReady.RemoveAt(Index);

	//Updates player controllers array
	UpdateControllersArray();
	
	Super::Logout(ExitingController);
}

void AOTLobbyGamemode::UpdateControllersArray()
{
	TArray<FString> TempPlayersArray;

	//Get players array to replicate it to all player controllers
	for(const APlayerController * Controller : PlayersConnected)
	{
		TempPlayersArray.Add(Controller->PlayerState->GetPlayerName());
	}

	//Replicated all states to player controllers
	for(APlayerController * Controller : PlayersConnected)
	{
		const auto LobbyController = Cast<AOTLobbyController>(Controller);
		LobbyController->PlayersName.Empty();
		LobbyController->PlayersName.Append(TempPlayersArray);
		LobbyController->PlayersReady.Empty();
		LobbyController->PlayersReady.Append(this->IsReady);

		//Call manually the event because ReplicatedUsing function is executed only on clients
		if(LobbyController->HasAuthority())
		{
			LobbyController->OnPlayerListModified();
		}
	}
}


void AOTLobbyGamemode::SetPlayerReady(AOTLobbyController* PlayerController, bool bIsReady)
{
	if(!ensureMsgf(PlayerController,TEXT("SetPlayerReady function need the player controller"))) return;
	const auto Index = PlayersConnected.Find(PlayerController);

	//Modifying ready state to other clients
	IsReady[Index] = bIsReady;
	PlayerController->bIsReady = bIsReady;
	
	//Call manually the event because ReplicatedUsing function is executed only on clients
	if(PlayerController->HasAuthority())
	{
		PlayerController->OnReadyModified();
	}
	
	//Updates player controllers array
	UpdateControllersArray();
	
	//Checking if all players are ready
	bool bAllReady = true;
	for(int i = 0; i < PlayersConnected.Num(); i++)
	{
		bAllReady&=IsReady[i];
	}
	if(bAllReady)
	{
		StartLobby();
	}
}

void AOTLobbyGamemode::StartLobby_Implementation()
{
	FString LevelPath;
	
	//Retrieve the path to the level
	GameLevel.ToSoftObjectPath().ToString().Split(FString("."),&LevelPath, nullptr );

	if(auto * World = GetWorld())
	{
		World->ServerTravel(LevelPath.Append("?listen"));
	}
	
}

void AOTLobbyGamemode::InitReadyArray()
{
	const auto * GameInstance = GetGameInstance();
	checkf(GameInstance,TEXT("Can't retrieve the game instance"));

	const auto * SessionSubsystem = GameInstance->GetSubsystem<UOTSessionsSubsystem>();
	checkf(SessionSubsystem,TEXT("Can't retrieve the session subsystem"));


	//Getting the session to get the max number of players
	const FNamedOnlineSession * CurrentSession  = SessionSubsystem->GetCurrentSession();
	if(!CurrentSession) return;

	const int32 MaxNumberOfPlayers = CurrentSession->SessionSettings.NumPublicConnections;
	
	IsReady.Init(false,MaxNumberOfPlayers);

}
