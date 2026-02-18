// Scientific Ways

#include "ComboSystem/ScWComboStateComponent.h"

#include "AbilitySystem/ScWAbilitySystemComponent.h"

#include "Tags/ScWComboTags.h"

#include "ComboSystem/ScWComboData.h"
#include "ComboSystem/ScWComboMoveData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWComboStateComponent)

UScWComboStateComponent::UScWComboStateComponent()
{
	CurrentComboState = FScWComboTags::Combo_State_Reset;
}

//~ Begin Statics
UScWComboStateComponent* UScWComboStateComponent::GetComboStateComponentFromActor(const AActor* InActor)
{
	return InActor ? InActor->FindComponentByClass<UScWComboStateComponent>() : nullptr;
}

UScWComboStateComponent* UScWComboStateComponent::GetComboStateComponentFromActorInfo(const FGameplayAbilityActorInfo& InActorInfo)
{
	if (UScWComboStateComponent* OwnerComboStateComponent = GetComboStateComponentFromActor(InActorInfo.OwnerActor.Get()))
	{
		return OwnerComboStateComponent;
	}
	else if (UScWComboStateComponent* AvatarComboStateComponent = GetComboStateComponentFromActor(InActorInfo.AvatarActor.Get()))
	{
		return AvatarComboStateComponent;
	}
	else
	{
		return nullptr;
	}
}
//~ End Statics

//~ Begin Initialize
void UScWComboStateComponent::OnRegister() // UActorComponent
{
	Super::OnRegister();

	
}

void UScWComboStateComponent::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	ensureReturn(IsValid());
}

/*void UScWComboStateComponent::TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) // UActorComponent
{
	Super::TickComponent(InDeltaTime, InTickType, InThisTickFunction);

	if (TempStateDurationTimeLeft == 0.0f)
	{

	}
	else
	{
		TempStateDurationTimeLeft = (TempStateDurationTimeLeft - InDeltaTime);

		if (TempStateDurationTimeLeft <= 0.0f)
		{
			TempStateDurationTimeLeft = 0.0f;
			SetComboState(FScWComboTags::Combo_State_Reset);
		}
	}
}*/
//~ End Initialize

//~ Begin Combo
bool UScWComboStateComponent::CanQueueNextComboMove() const
{
	return (CurrentComboState == FScWComboTags::Combo_State_Reset) || (CurrentComboState == FScWComboTags::Combo_State_ReadyForMove);
}

bool UScWComboStateComponent::TryQueueComboMove(const UScWComboMoveData* InComboMoveData)
{
	ensureReturn(InComboMoveData, false);

	if (CurrentComboState == FScWComboTags::Combo_State_Reset)
	{
		// Can queue, continue
	}
	else if (CurrentComboState == FScWComboTags::Combo_State_ReadyForMove)
	{
		// Can queue, continue
	}
	else if (CurrentComboState == FScWComboTags::Combo_State_NotYetReadyForMove)
	{
		// Can't queue, fail and cancel
		SetComboState(FScWComboTags::Combo_State_Failed);
		return false;
	}
	else if (CurrentComboState == FScWComboTags::Combo_State_Failed)
	{
		// Can't queue, skip
		return false;
	}
	else
	{
		// Invalid combo state, assert and skip
		ensureReturn(false, false);
	}
	QueuedComboMove = InComboMoveData;
	OnComboMoveQueuedDelegate.Broadcast(QueuedComboMove);
	return true;
}

void UScWComboStateComponent::AcceptQueuedComboMove()
{
	if (QueuedComboMove)
	{
		const UScWComboMoveData* AcceptedComboMove = QueuedComboMove;
		QueuedComboMove = nullptr;
		OnQueuedComboMoveAcceptedDelegate.Broadcast(AcceptedComboMove);

		AddComboMove(AcceptedComboMove, true, true);
	}
}

void UScWComboStateComponent::DenyQueuedComboMove()
{
	if (QueuedComboMove)
	{
		const UScWComboMoveData* DeniedComboMove = QueuedComboMove;
		QueuedComboMove = nullptr;
		OnQueuedComboMoveDeniedDelegate.Broadcast(DeniedComboMove);
	}
}

void UScWComboStateComponent::AddComboMove(const UScWComboMoveData* InComboMoveData, bool InUpdateRelevantCombo, bool InResetIfNoRelevantCombo)
{
	CurrentComboMoves.Add(InComboMoveData);
	OnComboMoveAddedDelegate.Broadcast(InComboMoveData);

	if (InUpdateRelevantCombo)
	{
		bool IsNewComboRelevant = UpdateRelevantComboFromCurrentMoves(InResetIfNoRelevantCombo);

		if (InResetIfNoRelevantCombo && !IsNewComboRelevant) // Probably went full cycle, so try to restart combo
		{
			AddComboMove(InComboMoveData, InUpdateRelevantCombo, false);
		}
	}
}

bool UScWComboStateComponent::SetComboState(const FGameplayTag& InState, bool InUpdateRelevantComboOnResetOrFail)
{
	if (CurrentComboState == InState)
	{
		return false;
	}
	UScWAbilitySystemComponent* OwnerASC = UScWAbilitySystemComponent::TryGetFromActor(GetOwner());
	if ensure(OwnerASC)
	{
		OwnerASC->RemoveDynamicTagGameplayEffect(CurrentComboState);
		OwnerASC->AddDynamicTagGameplayEffect(InState);
	}
	CurrentComboState = InState;
	//TempStateDurationTimeLeft = 0.0f;
		
	if (CurrentComboState == FScWComboTags::Combo_State_Reset)
	{
		CurrentComboMoves.Empty();

		if (InUpdateRelevantComboOnResetOrFail && !AvailableCombos.IsEmpty())
		{
			UpdateRelevantComboFromCurrentMoves(false);
		}
	}
	else if (CurrentComboState == FScWComboTags::Combo_State_ReadyForMove)
	{
			
	}
	else if (CurrentComboState == FScWComboTags::Combo_State_NotYetReadyForMove)
	{
			
	}
	else if (CurrentComboState == FScWComboTags::Combo_State_Failed)
	{
		CurrentComboMoves.Empty();

		if (InUpdateRelevantComboOnResetOrFail && !AvailableCombos.IsEmpty())
		{
			UpdateRelevantComboFromCurrentMoves(false);
		}
		if (ComboFailedOwnerEffectClass)
		{
			ApplyComboFailedOwnerEffect();
		}
		//TempStateDurationTimeLeft = 0.2f;
	}
	else
	{
		checkNoEntry();
	}
	OnComboStateChangedDelegate.Broadcast(CurrentComboState);
	return true;
}

/*bool UScWComboStateComponent::SetReadyForMoveStateForDuration(float InDuration)
{
	if (SetComboState(FScWComboTags::Combo_State_ReadyForMove))
	{
		TempStateDurationTimeLeft = InDuration;
		return true;
	}
	return false;
}*/

bool UScWComboStateComponent::UpdateRelevantComboFromCurrentMoves(bool InResetIfNoRelevantCombo)
{
	auto PrevRelevantCombo = RelevantCombo;
	RelevantCombo = nullptr;

	bool OutSuccess = false;

	ensureReturn(IsValid(), false);
	for (auto SampleCombo : AvailableCombos)
	{
		if (SampleCombo->Moves == CurrentComboMoves)
		{
			RelevantCombo = SampleCombo;
			OutSuccess = true;
			break;
		}
	}
	if (RelevantCombo != PrevRelevantCombo)
	{
		OnRelevantComboUpdated.Broadcast(OutSuccess);
	}
	if (InResetIfNoRelevantCombo && !OutSuccess)
	{
		if (CurrentComboState != FScWComboTags::Combo_State_Reset)
		{
			SetComboState(FScWComboTags::Combo_State_Reset, false);
		}
		else
		{
			CurrentComboMoves.Empty();
		}
	}
	return OutSuccess;
}
//~ End Combo

//~ Begin Effects
FActiveGameplayEffectHandle UScWComboStateComponent::ApplyComboFailedOwnerEffect()
{
	ensureReturn(ComboFailedOwnerEffectClass, FActiveGameplayEffectHandle());

	UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	ensureReturn(OwnerASC, FActiveGameplayEffectHandle());
	return OwnerASC->BP_ApplyGameplayEffectToSelf(ComboFailedOwnerEffectClass, 0.0f, OwnerASC->MakeEffectContext());
}
//~ End Effects
