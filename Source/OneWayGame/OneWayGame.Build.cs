// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OneWayGame : ModuleRules
{
	public OneWayGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"NavigationSystem",
			"OnlineSubsystem",
			"OnlineSubsystemSteam"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"OneWayGame",
			"OneWayGame/Variant_Platforming",
			"OneWayGame/Variant_Platforming/Animation",
			"OneWayGame/Variant_Combat",
			"OneWayGame/Variant_Combat/AI",
			"OneWayGame/Variant_Combat/Animation",
			"OneWayGame/Variant_Combat/Gameplay",
			"OneWayGame/Variant_Combat/Interfaces",
			"OneWayGame/Variant_Combat/UI",
			"OneWayGame/Variant_SideScrolling",
			"OneWayGame/Variant_SideScrolling/AI",
			"OneWayGame/Variant_SideScrolling/Gameplay",
			"OneWayGame/Variant_SideScrolling/Interfaces",
			"OneWayGame/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
