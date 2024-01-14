// UnrealEngine Starter template - by TechNet 2023

#include "Controllers/BasePlayerController.h"
#include "EnhancedInputSubsystems.h"

ABasePlayerController::ABasePlayerController()
{
	
	// With this template, this is the player controller that handle inputs so you have to enable the tick.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	checkf(MappingContext, TEXT("Mapping context is not set for BRPlayablePlayerController."));

	// set the mapping context
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem)
	{
	    // Subsystem is valid : this is the local player
		Subsystem->AddMappingContext(MappingContext, 0);
	}
	//else : this is not the local player, do nothing


	// Set Input mode
	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(true); // don't play primary action if it's a lock mouse action.
	SetInputMode(InputModeData);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	auto* TaggedInputComponent = CastChecked<UBaseTaggedInputComponent>(InputComponent);
	// Same as making a check after the Cast.

	// CHOSE A METHODE TO BIND INPUT-ACTION TO METHODS (1) OR TO TAG (2)(With ability system)
	// Some game will use both
	// 1 - binding action to a method
	// TaggedInputComponent->BindAction(MoveInputAction,ETriggerEvent::Triggered, this,  &ThisClass::InputMove);
	// 2 - binding DataAsset of tagged-actions to three methods
	// TaggedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::OnAbilityInputTagPressed, &ThisClass::OnAbilityInputTagReleased, &ThisClass::OnAbilityInputTagHeld);

	TaggedInputComponent->BindAction(MoveInputAction,ETriggerEvent::Triggered, this,  &ThisClass::InputMove);
	TaggedInputComponent->BindAction(LookInputAction,ETriggerEvent::Triggered, this,  &ThisClass::InputLook);
}

void ABasePlayerController::OnAbilityInputTagPressed(FGameplayTag InputTag)
{
	// Nothing to to here => when pressed the held event is called too
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Yellow, *InputTag.ToString());
}

void ABasePlayerController::OnAbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, *InputTag.ToString());
}

void ABasePlayerController::OnAbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Orange, *InputTag.ToString());
}


void ABasePlayerController::InputMove(const FInputActionValue& InputActionValue)
{
	// prevent useless operation
	const FVector2d MoveVector = InputActionValue.Get<FVector2d>();
	if (MoveVector.SquaredLength() == 0) return;

	//Get the forward direction from the controller
	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector ForwardDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
	const FVector RightDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MoveVector.Y); // forward direction
		ControlledPawn->AddMovementInput(RightDirection, -MoveVector.X); // right direction
	}
}

void ABasePlayerController::InputLook(const FInputActionValue& InputActionValue)
{
	const auto LookVector = InputActionValue.Get<FVector2d>();
	AddYawInput(LookVector.X);
	AddPitchInput(-LookVector.Y);	
}

