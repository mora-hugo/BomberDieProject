// UnrealEngine Starter template - by TechNet 2023


#include "Playground/BDPlaygroundGenerator.h"


// Sets default values
ABDPlaygroundGenerator::ABDPlaygroundGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABDPlaygroundGenerator::BeginPlay()
{
	Super::BeginPlay();
	if(bShouldGenerateOnBeginPlay) Generate();
}

void ABDPlaygroundGenerator::Generate()
{
	// Clear previous walls
	for(const auto Wall : Walls)
		Wall->Destroy();

	Walls.Empty();
	
	// Debug print
	if(GEngine && bPrintDebug)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Starting playground generation..."));

	// Playground Generation
	for(int l = 0; l < PlaygroundLenght; ++l)
	{
		for(int w = 0; w < PlaygroundWidth; ++w)
		{
			AActor* WallSpawned;
			
			// Get location to spawn wall
			const float x = CellSize*w;
			const float y = CellSize*l;
			const FVector NewWallLocation = GetActorLocation()+FVector(x,y,0);
			
			// skip if required
			FVector2f Wall2dLoc = FVector2f(l,w);
			if(WallsToNotSpawn.Contains(Wall2dLoc)) continue;

			// Check if is border raws or columns 
			if(w == 0 || w == PlaygroundWidth-1 || l == 0 || l == PlaygroundLenght-1)
			{
				//spawn destructible wall
				WallSpawned = GetWorld()->SpawnActor(DestructibleWall,&NewWallLocation);
			}
			// if not, spawns solid wall 1 cell out of 2
			else
			{
				WallSpawned = (l%2 == 0 && w%2 == 0)?
					GetWorld()->SpawnActor(SolidWall,&NewWallLocation)
					:
					GetWorld()->SpawnActor(DestructibleWall,&NewWallLocation);
			}

			// add the new wall to the walls list
			Walls.Add(WallSpawned);
			
		}
	}

	// Debug print
	if(GEngine && bPrintDebug)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("playground generation completed"));
}

