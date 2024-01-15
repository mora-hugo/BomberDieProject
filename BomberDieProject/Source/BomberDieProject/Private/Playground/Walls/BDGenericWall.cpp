// UnrealEngine Starter template - by TechNet 2023


#include "Playground/Walls/BDGenericWall.h"


// Sets default values
ABDGenericWall::ABDGenericWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	Shape->SetupAttachment(RootComponent);
	
}

void ABDGenericWall::Break()
{
}

// Called when the game starts or when spawned
void ABDGenericWall::BeginPlay()
{
	Super::BeginPlay();
	
}


