// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OTLobbyController.generated.h"

//
// -- Used for demonstration --
//

UCLASS()
class ONLINETOOLBOX_API AOTLobbyController : public APlayerController
{
	GENERATED_BODY()

	

protected:
	virtual void BeginPlay() override;

	virtual void OnNetCleanup(UNetConnection* Connection) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	
	
	UFUNCTION()
	void OnRep_IsReady(bool OldReady);

	UFUNCTION()
	void OnRep_PlayerList(TArray<FString> OldList);

	UFUNCTION()
	void OnRep_ReadyList(TArray<bool> OldReady);
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnReadyModified();

	UFUNCTION(BlueprintNativeEvent)
	void OnPlayerListModified();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerReady(bool bIsPlayerReady);


	//Our State, replicated to give client feedback
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_IsReady)
	bool bIsReady;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerList)
	TArray<FString> PlayersName;
	
	UPROPERTY(ReplicatedUsing=OnRep_ReadyList)
	TArray<bool> PlayersReady;

	//Use for blueprint access
	UPROPERTY(BlueprintReadOnly)
	TMap<FString,bool> PlayersState;
	
protected:

	UFUNCTION(Server,Reliable)
	void ServerUpdatePlayerReady(bool bIsPlayerReady);

	
	
};


