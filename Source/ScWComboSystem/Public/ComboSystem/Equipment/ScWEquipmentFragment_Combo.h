// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "Equipment/ScWEquipmentFragment_MeleeBase.h"

#include "ScWEquipmentFragment_Combo.generated.h"

/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Melee Equipment Fragment (Combo)"))
class UScWEquipmentFragment_Combo : public UScWEquipmentFragment_MeleeBase
{
	GENERATED_BODY()

public:	

	UScWEquipmentFragment_Combo();
	
//~ Begin Swing
public:
	virtual float BP_ModifySwingBaseDamage_Implementation(const class AActor* InEquipmentActor, float InDamage) const override; // UScWEquipmentFragment
	virtual TSubclassOf<class UScWDamageType> BP_ModifySwingBaseDamageTypeClass_Implementation(const class AActor* InEquipmentActor, TSubclassOf<class UScWDamageType> InDamageTypeClass) const override; // UScWEquipmentFragment

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float PostSwingComboTimeWindow;
//~ End Swing

//~ Begin Swing Variants
public:
	virtual void BP_ModifySwingVariants_Implementation(const class AActor* InEquipmentActor, TArray<FScWMeleeSwingVariantData>& InOutVariants) const override; // UScWEquipmentFragment
//~ End Swing Variants
};
