// Scientific Ways

#include "Gameplay/Combos/Tasks/ScWAT_WaitComboMoveEvent.h"

#include "Gameplay/Combos/ScWComboStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWAT_WaitComboMoveEvent)

//~ Begin Initialize
UScWAT_WaitComboMoveEvent* UScWAT_WaitComboMoveEvent::WaitComboMoveCommonInit(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent, int32 InEventType)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);
	OutTaskObject->ComboStateComponent = InComboStateComponent;
	OutTaskObject->EventType = InEventType;
	OutTaskObject->CallbackScriptDelegate.BindUFunction(OutTaskObject, TEXT("OnComboMoveEventCallback"));
	return OutTaskObject;
}

void UScWAT_WaitComboMoveEvent::Activate() // UGameplayTask
{
	if (ComboStateComponent)
	{
		switch (EventType)
		{
			case EVENT_TYPE_QUEUED: ComboStateComponent->OnComboMoveQueuedDelegate.Add(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_ACCEPTED: ComboStateComponent->OnQueuedComboMoveAcceptedDelegate.Add(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_DENIED: ComboStateComponent->OnQueuedComboMoveDeniedDelegate.Add(CallbackScriptDelegate); break;
			case EVENT_TYPE_ADDED: ComboStateComponent->OnComboMoveAddedDelegate.Add(CallbackScriptDelegate); break;
		}
	}
	else
	{
		ensureMsgf(false, TEXT("UScWAT_WaitComboMoveEvent::Activate() ComboStateComponent is null! Ending task immediately."));
		EndTask();
	}
}

void UScWAT_WaitComboMoveEvent::OnDestroy(bool bInAbilityIsEnding)
{
	if (ComboStateComponent)
	{
		switch (EventType)
		{
			case EVENT_TYPE_QUEUED: ComboStateComponent->OnComboMoveQueuedDelegate.Remove(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_ACCEPTED: ComboStateComponent->OnQueuedComboMoveAcceptedDelegate.Remove(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_DENIED: ComboStateComponent->OnQueuedComboMoveDeniedDelegate.Remove(CallbackScriptDelegate); break;
			case EVENT_TYPE_ADDED: ComboStateComponent->OnComboMoveAddedDelegate.Remove(CallbackScriptDelegate); break;
		}
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Callbacks
void UScWAT_WaitComboMoveEvent::OnComboMoveEventCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboMoveEvent.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
