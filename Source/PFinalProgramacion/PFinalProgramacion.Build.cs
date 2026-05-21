// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PFinalProgramacion : ModuleRules
{
	public PFinalProgramacion(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"PFinalProgramacion",
			"PFinalProgramacion/Variant_Platforming",
			"PFinalProgramacion/Variant_Platforming/Animation",
			"PFinalProgramacion/Variant_Combat",
			"PFinalProgramacion/Variant_Combat/AI",
			"PFinalProgramacion/Variant_Combat/Animation",
			"PFinalProgramacion/Variant_Combat/Gameplay",
			"PFinalProgramacion/Variant_Combat/Interfaces",
			"PFinalProgramacion/Variant_Combat/UI",
			"PFinalProgramacion/Variant_SideScrolling",
			"PFinalProgramacion/Variant_SideScrolling/AI",
			"PFinalProgramacion/Variant_SideScrolling/Gameplay",
			"PFinalProgramacion/Variant_SideScrolling/Interfaces",
			"PFinalProgramacion/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
