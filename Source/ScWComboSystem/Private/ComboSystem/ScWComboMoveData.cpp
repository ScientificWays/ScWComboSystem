// Scientific Ways

#include "ComboSystem/ScWComboMoveData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWComboMoveData)

UScWComboMoveData::UScWComboMoveData()
{
	DisplayName = FText::FromString(TEXT("Unnamed ComboMove"));

	MovePowerMul = 1.0f;

	MoveDamageMul = 1.0f;
	MoveDamageAdditive = 0.0f;
	MoveDamageImpactImpulseMul = 1.0f;
}
