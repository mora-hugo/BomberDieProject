// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Walls/BDGenericWall.h"
#include "BDPlaygroundGenerator.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API ABDPlaygroundGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABDPlaygroundGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Generate the playground
	 */
	UFUNCTION(CallInEditor, Category="Playground Generation")
	void Generate();

	UPROPERTY(EditAnywhere, Category="Playground Generation")
	TSubclassOf<ABDGenericWall> SolidWall;

	UPROPERTY(EditAnywhere, Category="Playground Generation")
	TSubclassOf<ABDGenericWall> DestructibleWall;

	UPROPERTY(EditAnywhere, Category="Playground Generation")
	int PlaygroundLenght = 10;

	UPROPERTY(EditAnywhere, Category="Playground Generation")
	int PlaygroundWidth = 10;
	
	UPROPERTY(EditAnywhere, Category="Playground Generation")
	float CellSize = 100.f;

	UPROPERTY(EditAnywhere, Category="Playground Generation")
	TArray<FVector2f> WallsToNotSpawn;
	
	UPROPERTY(EditAnywhere, Category="Playground Generation")
	bool bShouldGenerateOnBeginPlay = true;

	UPROPERTY(EditAnywhere, Category="Playground Generation")
	bool bPrintDebug = true;

private:

	UPROPERTY()
	TArray<AActor*> Walls;
};
