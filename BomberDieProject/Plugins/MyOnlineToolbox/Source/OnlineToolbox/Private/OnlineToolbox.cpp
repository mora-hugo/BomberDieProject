// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineToolbox.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#include "Settings/OTSettings.h"

#define LOCTEXT_NAMESPACE "FOnlineToolboxModule"

void FOnlineToolboxModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "Plugins", "Settings",
			LOCTEXT("RuntimeSettingsName", "Online Toolbox Settings"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the plugins"),
			GetMutableDefault<UOTSettings>()
		);
	}
}

void FOnlineToolboxModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Settings");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOnlineToolboxModule, OnlineToolbox)