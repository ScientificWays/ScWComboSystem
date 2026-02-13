// Scientific Ways

#pragma once

#include "ScWComboSystem.h"

#include "Abilities/Tasks/AbilityTask.h"

#include "ScWAT_WaitComboMoveEvent.generated.h"

/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitComboMoveEvent"))
class UScWAT_WaitComboMoveEvent : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnComboMoveEvent;

//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitComboMoveEvent* WaitComboMoveQueued(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent) { return WaitComboMoveCommonInit(InOwningAbility, InComboStateComponent, EVENT_TYPE_QUEUED); }

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitComboMoveEvent* WaitQueuedComboMoveAccepted(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent) { return WaitComboMoveCommonInit(InOwningAbility, InComboStateComponent, EVENT_TYPE_QUEUED_ACCEPTED); }

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitComboMoveEvent* WaitQueuedComboMoveDenied(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent) { return WaitComboMoveCommonInit(InOwningAbility, InComboStateComponent, EVENT_TYPE_QUEUED_DENIED); }

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitComboMoveEvent* WaitComboMoveAdded(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent) { return WaitComboMoveCommonInit(InOwningAbility, InComboStateComponent, EVENT_TYPE_ADDED); }

protected:
	static UScWAT_WaitComboMoveEvent* WaitComboMoveCommonInit(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent, int32 InEventType);

	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Callbacks
protected:

	UFUNCTION()
	void OnComboMoveEventCallback();

	UPROPERTY()
	TObjectPtr<UScWComboStateComponent> ComboStateComponent = nullptr;

	static const int32 EVENT_TYPE_QUEUED = 0;
	static const int32 EVENT_TYPE_QUEUED_ACCEPTED = 1;
	static const int32 EVENT_TYPE_QUEUED_DENIED = 2;
	static const int32 EVENT_TYPE_ADDED = 3;

	int32 EventType = EVENT_TYPE_QUEUED;
	FScriptDelegate CallbackScriptDelegate;
//~ End Callbacks
};
