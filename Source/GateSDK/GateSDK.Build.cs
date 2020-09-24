// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GateSDK : ModuleRules
{
	public GateSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
