// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "Equipment/ScWEquipmentActor_Melee.h"

#include "ScWComboData.generated.h"

#define MODULE_API SCWCOMBOSYSTEM_API

/**
 *
 */
UCLASS(Const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Combo Data"))
class UScWComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWComboData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image;
//~ End UI

//~ Begin Moves
public:

	UPROPERTY(Category = "Moves", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<const class UScWComboMoveData>> Moves;

	UPROPERTY(Category = "Moves", EditDefaultsOnly, BlueprintReadOnly)
	bool bUseLastMoveModifiers;
//~ End Moves
	
//~ Begin Power
public:

	UPROPERTY(Category = "Power", EditDefaultsOnly, BlueprintReadOnly)
	float ComboPowerMul;

	UFUNCTION(Category = "Power", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Equipment PowerMul"))
	MODULE_API float BP_GetEquipmentPowerMul(const AActor* InEquipmentActor) const;
//~ End Power

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Modify EquipmentDamage"))
	MODULE_API float BP_ModifyEquipmentDamage(const AActor* InEquipmentActor, float InDamage) const;

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Modify EquipmentDamageTypeClass"))
	MODULE_API TSubclassOf<class UScWDamageType> BP_ModifyEquipmentDamageTypeClass(const AActor* InEquipmentActor, TSubclassOf<class UScWDamageType> InDamageTypeClass) const;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageMul;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageAdditive;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> OverrideDamageTypeClass;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageImpactImpulseMul;
//~ End Damage

//~ Begin Swing Variants
public:

	UFUNCTION(Category = "Swing Variants", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Modify SwingVariants"))
	MODULE_API void BP_ModifySwingVariants(const AActor* InEquipmentActor, const TArray<FScWMeleeSwingVariantData>& InVariants, TArray<FScWMeleeSwingVariantData>& OutVariants) const;

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWMeleeSwingVariantData> OverrideSwingVariants;
//~ End Swing Variants
};

#undef MODULE_API
