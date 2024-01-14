// UnrealEngine Starter template - by TechNet 2023


#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Camera setup

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// ---------------------------------------------------------------------
// Camera
// ---------------------------------------------------------------------

USpringArmComponent* ABaseCharacter::GetSpringArm() const
{
	return SpringArm;
}

UCameraComponent* ABaseCharacter::GetCamera() const
{
	return Camera;
}


