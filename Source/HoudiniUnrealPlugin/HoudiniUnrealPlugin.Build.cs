// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HoudiniUnrealPlugin : ModuleRules
{
	public HoudiniUnrealPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
