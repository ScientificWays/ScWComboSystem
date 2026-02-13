// Scientific Ways

#include "ComboSystem/Abilities/ScWGameplayAbility_ComboSwing.h"

#include "AbilitySystem/Tasks/ScWAT_WaitDelay.h"

#include "ComboSystem/ScWComboData.h"
#include "ComboSystem/ScWComboStateComponent.h"
#include "ComboSystem/Tasks/ScWAT_WaitComboMoveEvent.h"
#include "ComboSystem/Equipment/ScWEquipmentFragment_Combo.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWGameplayAbility_ComboSwing)

UScWGameplayAbility_ComboSwing::UScWGameplayAbility_ComboSwing()
{
	
}

//~ Begin Ability
void UScWGameplayAbility_ComboSwing::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	ensureCancelAbilityReturn(InActorInfo);

	OwnerComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActorInfo(*InActorInfo);
	ensureCancelAbilityReturn(OwnerComboStateComponent);

	/*ensureCancelAbilityReturn(InTriggerEventData);
	const UScWComboMoveData* TriggeredComboMoveData = Cast<UScWComboMoveData>(InTriggerEventData->OptionalObject);
	ensureCancelAbilityReturn(TriggeredComboMoveData);

	switch (OwnerComboStateComponent->GetCurrentComboState())
	{
		case EComboState::Reset:
		{
			OwnerComboStateComponent->QueueComboMove(TriggeredComboMoveData);
			break;
		}
		case EComboState::ReadyForMove:
		{
			OwnerComboStateComponent->QueueComboMove(TriggeredComboMoveData);
			break;
		}
		case EComboState::NotYetReadyForMove:
		{
			OwnerComboStateComponent->SetComboState(EComboState::Failed);
			EndAbility(InHandle, InActorInfo, InActivationInfo, true, false);
			return;
		}
		case EComboState::Failed:
		{
			EndAbility(InHandle, InActorInfo, InActivationInfo, true, false);
			return;
		}
		default:
		{
			ensureCancelAbilityReturn(false);
			return;
		}
	}*/
	Super::ActivateAbility(InHandle, InActorInfo, InActivationInfo, InTriggerEventData);
}

void UScWGameplayAbility_ComboSwing::OnPostSwingComboWindowFinished()
{
	ensureCancelAbilityReturn(OwnerComboStateComponent);
	OwnerComboStateComponent->SetComboState(EComboState::Reset, true);
}

bool UScWGameplayAbility_ComboSwing::CanActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayTagContainer* InSourceTags, const FGameplayTagContainer* InTargetTags, OUT FGameplayTagContainer* OutOptionalRelevantTags) const // UGameplayAbility
{
	ensureReturn(InActorInfo, false);

	UScWComboStateComponent* ComboStateComponent = UScWComboStateComponent::GetComboStateComponentFromActorInfo(*InActorInfo);
	ensureReturn(ComboStateComponent, false);

	if (!ComboStateComponent->GetQueuedComboMove())
	{
		return false;
	}
	return Super::CanActivateAbility(InHandle, InActorInfo, InSourceTags, InTargetTags, OutOptionalRelevantTags);
}

void UScWGameplayAbility_ComboSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	if (bInWasCancelled)
	{
		OwnerComboStateComponent->DenyQueuedComboMove();
		OwnerComboStateComponent->SetComboState(EComboState::Reset, true);
	}
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
}
//~ End Ability

//~ Begin Swing
float UScWGameplayAbility_ComboSwing::BP_HandlePreSwing_Implementation() // UScWGameplayAbility_CommonSwing
{
	OwnerComboStateComponent->AcceptQueuedComboMove();

	ensureCancelAbilityReturn(OwnerComboStateComponent, Super::BP_HandlePreSwing_Implementation());
	OwnerComboStateComponent->SetComboState(EComboState::NotYetReadyForMove);

	return Super::BP_HandlePreSwing_Implementation();
}

float UScWGameplayAbility_ComboSwing::BP_HandleEndSwing_Implementation()
{
	float OutPostSwingDelay = Super::BP_HandleEndSwing_Implementation();

	BP_HandleSwingEndComboWindow();

	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor, OutPostSwingDelay);
	const UScWEquipmentFragment_Combo* ComboFragment = Cast<UScWEquipmentFragment_Combo>(OwnerMeleeEquipmentActor->GetMeleeFragment());

	ensureCancelAbilityReturn(ComboFragment, OutPostSwingDelay);
	return FMath::Max(ComboFragment->PostSwingComboTimeWindow, OutPostSwingDelay);
}

void UScWGameplayAbility_ComboSwing::BP_HandlePostSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerComboStateComponent);
	OwnerComboStateComponent->SetComboState(EComboState::Reset, true);

	Super::BP_HandlePostSwing_Implementation();
}

void UScWGameplayAbility_ComboSwing::BP_HandleSwingEndComboWindow_Implementation()
{
	UScWAT_WaitComboMoveEvent* WaitComboMoveQueuedTask = UScWAT_WaitComboMoveEvent::WaitComboMoveQueued(this, OwnerComboStateComponent);
	WaitComboMoveQueuedTask->OnComboMoveEvent.AddDynamic(this, &ThisClass::K2_EndAbility);
	WaitComboMoveQueuedTask->ReadyForActivation();

	ensureCancelAbilityReturn(OwnerComboStateComponent);
	OwnerComboStateComponent->SetComboState(EComboState::ReadyForMove);

	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor);
	const UScWEquipmentFragment_Combo* ComboFragment = Cast<UScWEquipmentFragment_Combo>(OwnerMeleeEquipmentActor->GetMeleeFragment());

	ensureCancelAbilityReturn(ComboFragment);
	COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PostSwingComboWindowTask, ComboFragment->PostSwingComboTimeWindow, OnPostSwingComboWindowFinished)
}
//~ End Swing
