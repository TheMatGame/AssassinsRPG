// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG_Assasin : ModuleRules
{
	public RPG_Assasin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "MotionWarping", "UMG", "Slate", "SlateCore" });
		
	}
}
