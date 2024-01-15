// UnrealEngine Starter template - by TechNet 2023


#include "GameInstances/BaseGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"


void UBaseGameInstance::Init()
{
	Super::Init();
	const auto Engine = GetEngine();
	check(Engine);
	
	// Bind Network Error handler
	NetworkFailureHandler = Engine->OnNetworkFailure().AddUObject(this, &UBaseGameInstance::HandleNetworkFailure);

	// Bind Travel Error handler
	TravelFailureHandler = Engine->OnTravelFailure().AddUObject(this, &UBaseGameInstance::HandleTravelError);
}

void UBaseGameInstance::Shutdown()
{
	Super::Shutdown();
	const auto Engine = GetEngine();
	check(Engine);
	
	if (NetworkFailureHandler.IsValid())
	{
		// Unbind Network Error handler
		Engine->OnNetworkFailure().Remove(NetworkFailureHandler);
		NetworkFailureHandler.Reset();
	}
	if (TravelFailureHandler.IsValid())
	{
		// Unbind Network Error handler
		Engine->OnTravelFailure().Remove(TravelFailureHandler);
		TravelFailureHandler.Reset();
	}
}


void UBaseGameInstance::OnDestroySessionCompleteHandle(FName SessionName, bool bWasSuccessful) const
{
	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(this->GetWorld());
	const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();

	SessionInterface->OnDestroySessionCompleteDelegates.RemoveAll(this);
	FString* MenuLevelPath = new FString();
	// 1 - Set the new level lobby path
	MainMenuLevel.ToSoftObjectPath().ToString().Split(FString("."), MenuLevelPath, nullptr);
	// 2 - Open the main menu level
	UGameplayStatics::OpenLevel(this, FName(**MenuLevelPath), true, FString("Error="+LastError));
	
}

void UBaseGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
                                           const FString& Error)
{
	UE_LOG(LogNet, Error, TEXT("Network error received : %s"), *Error);
	LastError = Error;
	
	ClearSession();
}

void UBaseGameInstance::HandleTravelError(UWorld* World, ETravelFailure::Type TravelFailure, const FString& Error)
{
	UE_LOG(LogNet, Error, TEXT("Travel error received : %s"), *Error);
	LastError = Error;

	ClearSession();
}

void UBaseGameInstance::ClearSession()
{
	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(this->GetWorld());
	const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBaseGameInstance::OnDestroySessionCompleteHandle);
	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		// Call did not start : Open the main menu level
		OnDestroySessionCompleteHandle(NAME_GameSession, false);
	}
}