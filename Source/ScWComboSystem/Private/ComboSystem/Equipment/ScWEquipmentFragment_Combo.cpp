// Scientific Ways

#include "ComboSystem/Equipment/ScWEquipmentFragment_Combo.h"

#include "ComboSystem/ScWComboData.h"
#include "ComboSystem/ScWComboStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentFragment_Combo)

UScWEquipmentFragment_Combo::UScWEquipmentFragment_Combo()
{
	PostSwingComboTimeWindow = 0.4f;
}

//~ Begin Swing
float UScWEquipmentFragment_Combo::BP_ModifySwingBaseDamage_Implementation(const AActor* InEquipmentActor, float InDamage) const // UScWEquipmentFragment
{
	float OutDamage = InDamage;
	ensureReturn(InEquipmentActor, OutDamage);

	UScWComboStateComponent* OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActor(InEquipmentActor->GetOwner());
	ensureReturn(OwnerComboStateComponent, OutDamage);

	if (const UScWComboData* RelevantCombo = OwnerComboStateComponent->GetRelevantCombo())
	{
		OutDamage = RelevantCombo->BP_ModifyEquipmentDamage(InEquipmentActor, OutDamage);
	}
	return OutDamage;
}

TSubclassOf<class UScWDamageType> UScWEquipmentFragment_Combo::BP_ModifySwingBaseDamageTypeClass_Implementation(const AActor* InEquipmentActor, TSubclassOf<class UScWDamageType> InDamageTypeClass) const // UScWEquipmentFragment
{
	TSubclassOf<class UScWDamageType> OutDamageTypeClass = InDamageTypeClass;
	ensureReturn(InEquipmentActor, OutDamageTypeClass);

	UScWComboStateComponent* OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActor(InEquipmentActor->GetOwner());
	ensureReturn(OwnerComboStateComponent, OutDamageTypeClass);

	if (const UScWComboData* RelevantCombo = OwnerComboStateComponent->GetRelevantCombo())
	{
		OutDamageTypeClass = RelevantCombo->BP_ModifyEquipmentDamageTypeClass(InEquipmentActor, OutDamageTypeClass);
	}
	return OutDamageTypeClass;
}
//~ End Swing

//~ Begin Swing Variants
void UScWEquipmentFragment_Combo::BP_ModifySwingVariants_Implementation(const AActor* InEquipmentActor, TArray<FScWMeleeSwingVariantData>& InOutVariants) const // UScWEquipmentFragment
{
	ensureReturn(InEquipmentActor);

	UScWComboStateComponent* OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActor(InEquipmentActor->GetOwner());
	ensureReturn(OwnerComboStateComponent);

	const UScWComboData* RelevantCombo = OwnerComboStateComponent->GetRelevantCombo();
	if (RelevantCombo && !RelevantCombo->OverrideSwingVariants.IsEmpty())
	{
		InOutVariants = RelevantCombo->OverrideSwingVariants;
	}
}
//~ End Swing Variants
