// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "Abilities/Tasks/AbilityTask.h"

#include "ScWAT_WaitComboStateChanged.generated.h"

/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitComboStateChanged"))
class UScWAT_WaitComboStateChanged : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnComboStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitComboStateChanged* WaitComboStateChanged(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent);
	
//~ Begin Initialize
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Callbacks
protected:

	UFUNCTION()
	void OnComboStateChangedCallback();

	UPROPERTY()
	UScWComboStateComponent* ComboStateComponent = nullptr;

	FScriptDelegate CallbackScriptDelegate;
//~ End Callbacks
};
