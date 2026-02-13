// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "Equipment/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "ScWGameplayAbility_ComboSwing.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Combo Swing Gameplay Ability"))
class UScWGameplayAbility_ComboSwing : public UScWGameplayAbility_CommonSwing
{

	GENERATED_BODY()

public:

	UScWGameplayAbility_ComboSwing();

//~ Begin Ability
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UScWGameplayAbility

	UFUNCTION()
	void OnPostSwingComboWindowFinished();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayTagContainer* InSourceTags = nullptr, const FGameplayTagContainer* InTargetTags = nullptr, OUT FGameplayTagContainer* OutOptionalRelevantTags = nullptr) const override; // UGameplayAbility
	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Swing
protected:
	virtual float BP_HandlePreSwing_Implementation() override; // UScWGameplayAbility_CommonSwing
	virtual float BP_HandleEndSwing_Implementation() override; // UScWGameplayAbility_CommonSwing
	virtual void BP_HandlePostSwing_Implementation() override; // UScWGameplayAbility_CommonSwing
	
	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle SwingEndCombo"))
	void BP_HandleSwingEndComboWindow();
//~ End Swing

//~ Begin Owner
protected:

	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class UScWComboStateComponent> OwnerComboStateComponent;
//~ End Owner
};
