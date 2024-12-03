// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NBodySimulation : ModuleRules
{
	public NBodySimulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		
		PublicIncludePaths.AddRange(
			new string[] {
				"NBodySimulation"
			}
		);

		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore" ,
			
			
			// The following are for JSON 
			"Json", "JsonUtilities"

		});

		PrivateDependencyModuleNames.AddRange(new string[] { "NBodySim", "RHI" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
