// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "Abilities/Tasks/AbilityTask.h"

#include "ScWAT_WaitComboStateChanged.generated.h"

#define MODULE_API SCWCOMBOSYSTEM_API

/**
 *	Ability task that fires when the combo state component transitions to a new state.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitComboStateChanged"))
class UScWAT_WaitComboStateChanged : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnComboStateChanged;

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
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
	TObjectPtr<UScWComboStateComponent> ComboStateComponent = nullptr;

	FScriptDelegate CallbackScriptDelegate;
//~ End Callbacks
};

#undef MODULE_API
