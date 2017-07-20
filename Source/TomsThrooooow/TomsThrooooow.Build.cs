// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TomsThrooooow : ModuleRules
{
	public TomsThrooooow(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});

        PublicDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });
        //Load Steam Online Module
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}
