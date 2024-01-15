// UnrealEngine Starter template - by TechNet 2023


#include "Weapons/BDBomb.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABDBomb::ABDBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TimerHandle.Invalidate();

	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;

	BoxCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABDBomb::BeginPlay()
{
	Super::BeginPlay();

	if(!HasAuthority()) return;

	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABDBomb::Explode, Distance);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, MyDelay, false);
}

void ABDBomb::Explode(int ExplosionLength)	// This function is called by the TimerHandle
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	DisplayBombFX();

	bIsExploded = true;
	ExplosionLength = ExplosionLength * DISTANCE_UNIT;
	TArray<FHitResult> Results;

	Results.Append(BombTraceChannel(true, ExplosionLength));
	Results.Append(BombTraceChannel(true, -ExplosionLength));
	Results.Append(BombTraceChannel(false, ExplosionLength));
	Results.Append(BombTraceChannel(false, -ExplosionLength));

	if (!Results.IsEmpty())
	{
		for (auto& Hit : Results)
		{
			UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, (GetInstigator() ? GetInstigator()->GetController() : nullptr), this, DamageType);
		}
	}


	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(ExplosionLength, 0, 0), FColor::Red, false, 5.f, 0, 1.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, ExplosionLength, 0), FColor::Red, false, 5.f, 0, 1.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(- ExplosionLength, 0, 0), FColor::Red, false, 5.f, 0, 1.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, - ExplosionLength, 0), FColor::Red, false, 5.f, 0, 1.f);

	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("Explode !"));
	UE_LOG(LogTemp, Warning, TEXT("UNIT is : %d !"), ExplosionLength);
}

TArray<FHitResult> ABDBomb::BombTraceChannel(bool AxeX, int ExplosionLength)
{
	TArray<FHitResult> Results;

	if (GetWorld()->SweepMultiByChannel(
		OutHitResult,
		GetActorLocation(),
		GetActorLocation() + (AxeX ? FVector(ExplosionLength, 0, 0) : FVector(0, ExplosionLength, 0)),
		FQuat::Identity,
		BombCollisionChannel,
		FCollisionShape::MakeSphere(DISTANCE_UNIT / 2),
		FCollisionQueryParams::DefaultQueryParam,
		FCollisionResponseParams::DefaultResponseParam))
	{
		Results = OutHitResult;
	}
	return Results;
}

void ABDBomb::DisplayBombFX_Implementation() const
{
	//To be continued...
}

