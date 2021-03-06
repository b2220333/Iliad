// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

using UnrealBuildTool;

public class OdysseyBrush : ModuleRules
{
    public OdysseyBrush(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "OdysseyStrokePipeline",
                "OdysseyImaging",
                "ULIS",
                "ULISLoader",
            }
        );
    }
}
