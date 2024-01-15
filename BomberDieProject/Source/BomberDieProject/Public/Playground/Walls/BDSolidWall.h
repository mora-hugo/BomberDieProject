// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "BDGenericWall.h"
#include "BDSolidWall.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API ABDSolidWall : public ABDGenericWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABDSolidWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
