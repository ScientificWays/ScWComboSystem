// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "ScWComboMoveData.generated.h"

#define MODULE_API SCWCOMBOSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Combo Move Data"))
class UScWComboMoveData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UScWComboMoveData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image;
//~ End UI
	
//~ Begin Power
public:

	UPROPERTY(Category = "Power", EditDefaultsOnly, BlueprintReadOnly)
	float MovePowerMul;
//~ End Power
	
//~ Begin Damage
public:

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float MoveDamageMul;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float MoveDamageAdditive;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> OverrideMoveDamageTypeClass;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float MoveDamageImpactImpulseMul;
//~ End Damage
};

#undef MODULE_API
