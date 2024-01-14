// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Inputs/BaseTaggedInputComponent.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERDIEPROJECT_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

protected:
	virtual void BeginPlay() override;

	// Base input actions
	virtual void SetupInputComponent() override;
	void OnAbilityInputTagPressed(FGameplayTag InputTag);
	void OnAbilityInputTagReleased(FGameplayTag InputTag);
	void OnAbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditAnywhere, Category="Input");
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookInputAction;

	/**
	 * @brief Move the character when input is triggered
	 * @param InputActionValue 
	 */
	void InputMove(const FInputActionValue& InputActionValue);
	/**
	 * @brief the character camera movement
	 * @param InputActionValue 
	 */
	void InputLook(const FInputActionValue& InputActionValue);
	
	// Input components
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;
	// Used to get relation between tag and inputs
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UBaseInputConfig> InputConfig;
};
