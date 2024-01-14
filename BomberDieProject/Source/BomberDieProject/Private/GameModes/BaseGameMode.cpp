// UnrealEngine Starter template - by TechNet 2023


#include "GameModes/BaseGameMode.h"


// Sets default values
ABaseGameMode::ABaseGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

