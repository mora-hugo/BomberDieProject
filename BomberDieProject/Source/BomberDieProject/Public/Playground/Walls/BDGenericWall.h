// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDGenericWall.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API ABDGenericWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABDGenericWall();

	// Break the Wall
	virtual void Break();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Shape;
};
