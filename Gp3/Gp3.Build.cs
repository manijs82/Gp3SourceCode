// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gp3 : ModuleRules
{
	public Gp3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG" });
    }
}
