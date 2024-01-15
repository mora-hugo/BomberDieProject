// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BomberDieProject : ModuleRules
{
	public BomberDieProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "UMG", "OnlineToolbox", "GeometryCore", "BaseSettingsSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"OnlineSubsystemEOS"
		});

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
