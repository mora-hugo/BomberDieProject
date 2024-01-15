// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDBomb.generated.h"

#define DISTANCE_UNIT 100

UCLASS()
class BOMBERDIEPROJECT_API ABDBomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABDBomb();

	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void Explode(int Distance);

	UFUNCTION()
	TArray<FHitResult> BombTraceChannel(bool AxeX, int ExplosionLength);

	UFUNCTION(NetMulticast, Reliable, Category = "Bomb") // This function is called by the server and played on all clients, Reliable is like UDP
	void DisplayBombFX() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	class UStaticMeshComponent* BombMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	class USphereComponent* BoxCollision;

	UPROPERTY(BlueprintReadOnly)
	bool bIsExploded = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	int Distance = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	float MyDelay = 3.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	float Damage = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bomb")
	TEnumAsByte<ECollisionChannel> BombCollisionChannel;


private:
	UPROPERTY()
	TArray<FHitResult> OutHitResult;

	UPROPERTY()
	FTimerHandle TimerHandle;
};
