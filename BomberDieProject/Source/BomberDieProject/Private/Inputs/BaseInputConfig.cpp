// UnrealEngine Starter template - by TechNet 2023


#include "Inputs/BaseInputConfig.h"

const UInputAction* UBaseInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FBaseTaggedInputAction& TaggedInputAction : TaggedInputActions)
	{
		if (TaggedInputAction.InputAction && TaggedInputAction.InputTag == InputTag)
		{
			return TaggedInputAction.InputAction;
		}
	}

	return nullptr;
}