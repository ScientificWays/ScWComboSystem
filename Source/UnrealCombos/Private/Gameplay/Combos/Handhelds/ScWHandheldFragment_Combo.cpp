// Scientific Ways

#include "Gameplay/Combos/Handhelds/ScWHandheldFragment_Combo.h"

#include "Gameplay/Combos/ScWComboData.h"
#include "Gameplay/Combos/ScWComboStateComponent.h"

#include "Gameplay/Handhelds/ScWHandheld.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWHandheldFragment_Combo)

UScWHandheldFragment_Combo::UScWHandheldFragment_Combo()
{
	PostSwingComboTimeWindow = 0.4f;
}

//~ Begin Swing
float UScWHandheldFragment_Combo::BP_ModifySwingBaseDamage_Implementation(const AScWHandheld* InHandheld, float InDamage) const // UScWHandheldFragment
{
	float OutDamage = InDamage;
	ensureReturn(InHandheld, OutDamage);

	UScWComboStateComponent* OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActor(InHandheld->GetOwner());
	ensureReturn(OwnerComboStateComponent, OutDamage);

	if (const UScWComboData* RelevantCombo = OwnerComboStateComponent->GetRelevantCombo())
	{
		OutDamage = RelevantCombo->BP_ModifyHandheldDamage(InHandheld, OutDamage);
	}
	return OutDamage;
}

TSubclassOf<class UScWDamageType> UScWHandheldFragment_Combo::BP_ModifySwingBaseDamageTypeClass_Implementation(const AScWHandheld* InHandheld, TSubclassOf<class UScWDamageType> InDamageTypeClass) const // UScWHandheldFragment
{
	TSubclassOf<class UScWDamageType> OutDamageTypeClass = InDamageTypeClass;
	ensureReturn(InHandheld, OutDamageTypeClass);

	UScWComboStateComponent* OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActor(InHandheld->GetOwner());
	ensureReturn(OwnerComboStateComponent, OutDamageTypeClass);

	if (const UScWComboData* RelevantCombo = OwnerComboStateComponent->GetRelevantCombo())
	{
		OutDamageTypeClass = RelevantCombo->BP_ModifyHandheldDamageTypeClass(InHandheld, OutDamageTypeClass);
	}
	return OutDamageTypeClass;
}
//~ End Swing

//~ Begin Swing Variants
void UScWHandheldFragment_Combo::BP_ModifySwingVariants_Implementation(const AScWHandheld* InHandheld, TArray<FScWMeleeSwingVariantData>& InOutVariants) const // UScWHandheldFragment
{
	ensureReturn(InHandheld);

	UScWComboStateComponent* OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActor(InHandheld->GetOwner());
	ensureReturn(OwnerComboStateComponent);

	const UScWComboData* RelevantCombo = OwnerComboStateComponent->GetRelevantCombo();
	if (RelevantCombo && !RelevantCombo->OverrideSwingVariants.IsEmpty())
	{
		InOutVariants = RelevantCombo->OverrideSwingVariants;
	}
}
//~ End Swing Variants
