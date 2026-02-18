// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "NativeGameplayTags.h"

#define MODULE_API SCWCOMBOSYSTEM_API

namespace FScWComboTags
{
	// Combo States
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_State_Reset);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_State_ReadyForMove);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_State_NotYetReadyForMove);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_State_Failed);
}

#undef MODULE_API
