// Scientific Ways

#include "ComboSystem/ScWComboStateComponent.h"

#include "Damage/ScWDamageType.h"

#include "AbilitySystem/ScWCoreTags.h"
#include "AbilitySystem/ScWASC_InitInterface.h"

#include "ComboSystem/ScWComboData.h"
#include "ComboSystem/ScWComboMoveData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWComboStateComponent)

UScWComboStateComponent::UScWComboStateComponent()
{
	
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
//~ End Initialize

//~ Begin Combo
bool UScWComboStateComponent::QueueComboMove(const UScWComboMoveData* InComboMoveData)
{
	ensureReturn(InComboMoveData, false);

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

void UScWComboStateComponent::SetComboState(EComboState InState, bool InUpdateRelevantComboOnResetOrFail)
{
	if (CurrentComboState != InState)
	{
		CurrentComboState = InState;

		switch (CurrentComboState)
		{
			case EComboState::Reset:
			{
				CurrentComboMoves.Empty();

				if (InUpdateRelevantComboOnResetOrFail && !AvailableCombos.IsEmpty())
				{
					UpdateRelevantComboFromCurrentMoves(false);
				}
				break;
			}
			case EComboState::ReadyForMove:
			{
				break;
			}
			case EComboState::NotYetReadyForMove:
			{
				break;
			}
			case EComboState::Failed:
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
				break;
			}
			default:
			{
				checkNoEntry();
				break;
			}
		}
		OnComboStateChangedDelegate.Broadcast(CurrentComboState);
	}
}

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
		if (CurrentComboState != EComboState::Reset)
		{
			SetComboState(EComboState::Reset, false);
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
