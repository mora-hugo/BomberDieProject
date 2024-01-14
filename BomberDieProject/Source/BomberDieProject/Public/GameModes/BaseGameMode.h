// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
