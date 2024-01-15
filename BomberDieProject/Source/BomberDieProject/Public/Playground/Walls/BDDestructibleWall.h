// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "BDGenericWall.h"
#include "BDDestructibleWall.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API ABDDestructibleWall : public ABDGenericWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABDDestructibleWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
