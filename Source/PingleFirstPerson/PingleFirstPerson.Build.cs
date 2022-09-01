// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PingleFirstPerson : ModuleRules
{
	public PingleFirstPerson(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
