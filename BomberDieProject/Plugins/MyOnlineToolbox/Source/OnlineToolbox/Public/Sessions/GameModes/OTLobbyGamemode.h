// Fill out your copyright notice in the Description page of Project Settings.

//
// -- Used for demonstration --
//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UObject/Object.h"
#include "OTLobbyGamemode.generated.h"

class AOTLobbyController;
/**
 * 
 */
UCLASS()
class ONLINETOOLBOX_API AOTLobbyGamemode : public AGameMode
{
	GENERATED_BODY()
	
	UFUNCTION()
	virtual void OnPostLogin(AController* NewController) override;
	
	UFUNCTION()
	virtual void Logout(AController* ExitingController) override;

	void UpdateControllersArray();

	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(AOTLobbyController* PlayerController, bool bIsReady);
protected:
	UPROPERTY(EditDefaultsOnly,Category="Levels")
	TSoftObjectPtr<UWorld> GameLevel;
	
	UFUNCTION(BlueprintNativeEvent)
	void StartLobby();
	
	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerController*> PlayersConnected;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> IsReady;

	void InitReadyArray();

};
