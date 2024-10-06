// Copyright Timothy Benton 2024


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		check(GameplayEffectClass);
		
		FGameplayEffectContextHandle GameplayEffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, GameplayEffectContextHandle);
		TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto GameplayEffect : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto GameplayEffect : DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto GameplayEffect : InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto GameplayEffect : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto GameplayEffect : DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
    {
		for (const auto GameplayEffect : InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
    }
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			for (const auto GameplayEffect : InfiniteGameplayEffectClasses)
			{
				TargetAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, TargetAbilitySystemComponent, 1);
			}
		}
	}
}
