// UnrealEngine Starter template - by TechNet 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputTriggers.h"
#include "InputAction.h"
#include "Inputs/BaseInputConfig.h"
#include "EnhancedInputComponent.h"
#include "BaseTaggedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERDIEPROJECT_API UBaseTaggedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UBaseInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

	template <class UserClass, typename FuncType>
	void BindActionByTag(const UBaseInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
	                     UserClass* Object, FuncType Func);
};

template <class UserClass, typename FuncType>
void UBaseTaggedInputComponent::BindActionByTag(const UBaseInputConfig* InputConfig, const FGameplayTag& InputTag,
                                                ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}


template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UBaseTaggedInputComponent::BindAbilityActions(const UBaseInputConfig* InputConfig, UserClass* Object,
                                                   PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                                   HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (auto [InputAction, InputTag] : InputConfig->TaggedInputActions)
	{
		if (InputAction && InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
			}
			if (HeldFunc)
			{
				BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
			}
		}
	}
}
