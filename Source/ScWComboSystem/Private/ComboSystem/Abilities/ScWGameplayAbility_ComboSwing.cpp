// Scientific Ways

#include "ComboSystem/Abilities/ScWGameplayAbility_ComboSwing.h"

#include "AbilitySystem/Tasks/ScWAT_WaitDelay.h"

#include "Tags/ScWComboTags.h"

#include "ComboSystem/ScWComboData.h"
#include "ComboSystem/ScWComboStateComponent.h"
#include "ComboSystem/Tasks/ScWAT_WaitComboMoveEvent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWGameplayAbility_ComboSwing)

UScWGameplayAbility_ComboSwing::UScWGameplayAbility_ComboSwing()
{
	PostSwingComboTimeWindow = 0.4f;
}

//~ Begin Ability
bool UScWGameplayAbility_ComboSwing::CommitAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, FGameplayTagContainer* InOutOptionalRelevantTags)
{
	ensureCancelAbilityReturn(InActorInfo, false);

	OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActorInfo(*InActorInfo);
	ensureCancelAbilityReturn(OwnerComboStateComponent, false);

	return Super::CommitAbility(InHandle, InActorInfo, InActivationInfo, InOutOptionalRelevantTags);
	/*if (OwnerComboStateComponent->TryQueueComboMove(ComboMove))
	{
		return Super::CommitAbility(InHandle, InActorInfo, InActivationInfo, InOutOptionalRelevantTags);
	}
	else
	{
		OwnerComboStateComponent->SetComboState(FScWComboTags::Combo_State_Failed, true);
		EndAbility(InHandle, InActorInfo, InActivationInfo, true, false);
		return false;
	}*/
}

void UScWGameplayAbility_ComboSwing::OnPostSwingComboWindowFinished()
{
	ensureCancelAbilityReturn(OwnerComboStateComponent);
	OwnerComboStateComponent->SetComboState(FScWComboTags::Combo_State_Reset, true);
}

void UScWGameplayAbility_ComboSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	if (bInWasCancelled)
	{
		if (OwnerComboStateComponent)
		{
			OwnerComboStateComponent->DenyQueuedComboMove();
			OwnerComboStateComponent->SetComboState(FScWComboTags::Combo_State_Reset, true);
		}
	}
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
}
//~ End Ability

//~ Begin Swing
void UScWGameplayAbility_ComboSwing::BeginSwingSequence() // UScWGameplayAbility_CommonSwing
{
	ensureCancelAbilityReturn(OwnerComboStateComponent);
	OwnerComboStateComponent->AcceptQueuedComboMove();

	Super::BeginSwingSequence();
}

float UScWGameplayAbility_ComboSwing::BP_HandlePreSwing_Implementation() // UScWGameplayAbility_CommonSwing
{
	ensureCancelAbilityReturn(OwnerComboStateComponent, Super::BP_HandlePreSwing_Implementation());
	OwnerComboStateComponent->SetComboState(FScWComboTags::Combo_State_NotYetReadyForMove);

	return Super::BP_HandlePreSwing_Implementation();
}

float UScWGameplayAbility_ComboSwing::BP_HandleEndSwing_Implementation()
{
	float OutPostSwingDelay = Super::BP_HandleEndSwing_Implementation();

	BP_HandleSwingEndComboWindow();
	return FMath::Max(PostSwingComboTimeWindow, OutPostSwingDelay);
}

void UScWGameplayAbility_ComboSwing::BP_HandlePostSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerComboStateComponent);
	OwnerComboStateComponent->SetComboState(FScWComboTags::Combo_State_Reset, true);

	Super::BP_HandlePostSwing_Implementation();
}

void UScWGameplayAbility_ComboSwing::BP_HandleSwingEndComboWindow_Implementation()
{
	// End ability to not block next combo abilities
	UScWAT_WaitComboMoveEvent* WaitComboMoveQueuedTask = UScWAT_WaitComboMoveEvent::WaitComboMoveQueued(this, OwnerComboStateComponent);
	WaitComboMoveQueuedTask->OnComboMoveEvent.AddDynamic(this, &ThisClass::K2_EndAbility);
	WaitComboMoveQueuedTask->ReadyForActivation();

	ensureCancelAbilityReturn(OwnerComboStateComponent);
	//OwnerComboStateComponent->SetReadyForMoveStateForDuration(PostSwingComboTimeWindow);
	OwnerComboStateComponent->SetComboState(FScWComboTags::Combo_State_ReadyForMove);

	COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PostSwingComboWindowTask, PostSwingComboTimeWindow, OnPostSwingComboWindowFinished);

	//K2_EndAbility();
}
//~ End Swing
