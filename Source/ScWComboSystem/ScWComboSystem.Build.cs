// Scientific Ways

using UnrealBuildTool;
using System.IO; // for Path

public class ScWComboSystem : ModuleRules
{
	public ScWComboSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",

			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",

			"Networking",

			"ScWGameCore",
			"ScWEquipmentSystem",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Engine",
			"CoreUObject",
		});
	}
}
