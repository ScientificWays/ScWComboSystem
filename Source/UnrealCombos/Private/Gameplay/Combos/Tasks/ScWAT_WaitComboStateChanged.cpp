// Scientific Ways

#include "Gameplay/Combos/Tasks/ScWAT_WaitComboStateChanged.h"

#include "Gameplay/Combos/ScWComboStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWAT_WaitComboStateChanged)

//~ Begin Initialize
UScWAT_WaitComboStateChanged* UScWAT_WaitComboStateChanged::WaitComboStateChanged(UGameplayAbility* InOwningAbility, UScWComboStateComponent* InComboStateComponent)
{
	UScWAT_WaitComboStateChanged* OutTaskObject = NewAbilityTask<UScWAT_WaitComboStateChanged>(InOwningAbility);
	OutTaskObject->ComboStateComponent = InComboStateComponent;
	OutTaskObject->CallbackScriptDelegate.BindUFunction(OutTaskObject, TEXT("OnComboStateChangedCallback"));
	return OutTaskObject;
}

void UScWAT_WaitComboStateChanged::Activate() // UGameplayTask
{
	if (ComboStateComponent)
	{
		ComboStateComponent->OnComboStateChangedDelegate.Add(CallbackScriptDelegate);
	}
	else
	{
		ensureMsgf(false, TEXT("UScWAT_WaitComboStateChanged::Activate() ComboStateComponent is null! Ending task immediately."));
		EndTask();
	}
}

void UScWAT_WaitComboStateChanged::OnDestroy(bool bInAbilityIsEnding)
{
	
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Callbacks
void UScWAT_WaitComboStateChanged::OnComboStateChangedCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboStateChanged.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
