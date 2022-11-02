// Some copyright should be here...

using UnrealBuildTool;
using System;
using System.IO;

public class HoudiniEngine : ModuleRules
{
	public HoudiniEngine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;
        bUseUnity = false;
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory,"Public/HAPI")
			}
			) ;
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ProceduralMeshComponent",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		PublicLibraryPaths.Add("C:\\Program Files\\Side Effects Software\\Houdini 18.0.287\\bin");
		PublicDelayLoadDLLs.Add("libHAPIL.dll");
		PublicAdditionalLibraries.Add("C:\\Program Files\\Side Effects Software\\Houdini 18.0.287\\custom\\houdini\\dsolib\\libHAPIL.lib");
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
