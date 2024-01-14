// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGameInstance.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UBaseGameInstance();

	virtual void Init() override;

	virtual void Shutdown() override;
};
