// Scientific Ways

#include "Gameplay/Combos/ScWComboData.h"

#include "Gameplay/Combos/ScWComboMoveData.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Handhelds/ScWHandheld.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWComboData)

UScWComboData::UScWComboData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Combo"));

	bUseLastMoveModifiers = true;

	ComboPowerMul = 1.0f;

	DamageMul = 1.0f;
	DamageAdditive = 0.0f;
	DamageImpactImpulseMul = 1.0f;
}

//~ Begin Power
float UScWComboData::BP_GetHandheldPowerMul_Implementation(const AScWHandheld* InHandheld) const
{
	auto OutPowerMul = ComboPowerMul;

	if (bUseLastMoveModifiers)
	{
		ensureReturn(!Moves.IsEmpty(), OutPowerMul);
		auto LastMove = Moves.Last();

		OutPowerMul *= LastMove->MovePowerMul;
	}
	return OutPowerMul;
}
//~ End Power

//~ Begin Damage
float UScWComboData::BP_ModifyHandheldDamage_Implementation(const AScWHandheld* InHandheld, float InDamage) const
{
	float FinalDamageMul = DamageMul;
	float FinalDamageAdditive = DamageAdditive;

	if (bUseLastMoveModifiers)
	{
		ensureReturn(!Moves.IsEmpty(), InDamage);
		auto LastMove = Moves.Last();

		FinalDamageMul *= LastMove->MoveDamageMul;
		FinalDamageAdditive += LastMove->MoveDamageAdditive;
	}
	return InDamage * FinalDamageMul + FinalDamageAdditive;
}

TSubclassOf<UScWDamageType> UScWComboData::BP_ModifyHandheldDamageTypeClass_Implementation(const AScWHandheld* InHandheld, TSubclassOf<UScWDamageType> InDamageTypeClass) const
{
	if (OverrideDamageTypeClass)
	{
		return OverrideDamageTypeClass;
	}
	else if (bUseLastMoveModifiers)
	{
		ensureReturn(!Moves.IsEmpty(), InDamageTypeClass);
		auto LastMove = Moves.Last();

		if (LastMove->OverrideMoveDamageTypeClass)
		{
			return LastMove->OverrideMoveDamageTypeClass;
		}
	}
	return InDamageTypeClass;
}
//~ End Damage

//~ Begin Swing Variants
void UScWComboData::BP_ModifySwingVariants_Implementation(const AScWHandheld* InHandheld, const TArray<FScWMeleeSwingVariantData>& InVariants, TArray<FScWMeleeSwingVariantData>& OutVariants) const
{
	OutVariants = OverrideSwingVariants.IsEmpty() ? InVariants : OverrideSwingVariants;
}
//~ End Swing Variants
