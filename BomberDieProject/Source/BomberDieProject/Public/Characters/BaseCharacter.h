// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class BOMBERDIEPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ---------------------------------------------------------------------
	// Camera
	// ---------------------------------------------------------------------

public:

	UFUNCTION(BlueprintCallable)
	class USpringArmComponent* GetSpringArm() const;

	UFUNCTION(BlueprintCallable)
	class UCameraComponent* GetCamera() const;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Camera")
	TObjectPtr<UCameraComponent> Camera;

};
