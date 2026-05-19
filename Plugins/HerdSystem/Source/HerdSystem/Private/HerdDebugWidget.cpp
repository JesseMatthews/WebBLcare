#include "HerdDebugWidget.h"

#include "HerdManager_Generic.h"
#include "HerdMemberComponent.h"
#include "SpeciesData_Base.h"

void UHerdDebugWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    RefreshTimer += InDeltaTime;
    if (RefreshTimer >= RefreshInterval)
    {
        RefreshTimer = 0.0f;
        Refresh();
    }
}

void UHerdDebugWidget::SetObservedManager(AHerdManager_Generic* InManager)
{
    ObservedManager = InManager;
    Refresh();
}

void UHerdDebugWidget::Refresh()
{
    MemberRows.Empty();
    HerdCompositionText = FText::GetEmpty();
    ThreatSummaryText = FText::GetEmpty();
    MigrationRouteText = FText::GetEmpty();

    if (!ObservedManager)
    {
        return;
    }

    FString Composition;
    for (const FHerdRuntimeSpeciesEntry& Entry : ObservedManager->SpeciesRuntime)
    {
        const FName SpeciesId = Entry.SpeciesData ? Entry.SpeciesData->SpeciesId : NAME_None;
        Composition += FString::Printf(TEXT("%s: %d  Health %.2f  Hunger %.2f  Thirst %.2f  Panic %.2f\n"),
            *SpeciesId.ToString(),
            Entry.CurrentCount,
            Entry.AverageHealth,
            Entry.AverageHunger,
            Entry.AverageThirst,
            Entry.AveragePanic);
    }
    HerdCompositionText = FText::FromString(Composition);

    for (UHerdMemberComponent* Member : ObservedManager->RegisteredMembers)
    {
        if (!Member)
        {
            continue;
        }

        FHerdDebugMemberRow Row;
        Row.Actor = Member->GetOwner();
        Row.SpeciesId = Member->SpeciesData ? Member->SpeciesData->SpeciesId : NAME_None;
        Row.State = Member->CurrentState;
        Row.LifeStage = Member->LifeStage;
        Row.Role = Member->HerdRole;
        Row.Health = Member->GetHealthRatio();
        Row.Hunger = Member->GetHungerRatio();
        Row.Thirst = Member->GetThirstRatio();
        Row.Panic = Member->Panic;
        Row.Fear = Member->Fear;
        if (Member->SpeciesData)
        {
            if (const UAnimSequenceBase* Animation = Member->SpeciesData->GetAnimationForState(Member->CurrentState))
            {
                Row.CurrentAnimation = Animation->GetFName();
            }
        }
        MemberRows.Add(Row);
    }

    const FHerdScanDataset ScanDataset = ObservedManager->GetCurrentScanDataset();
    ThreatSummaryText = FText::FromString(FString::Printf(TEXT("Threats: %d  Food: %d  Water: %d  Species: %d"),
        ScanDataset.NearbyThreats.Num(),
        ScanDataset.NearbyFood.Num(),
        ScanDataset.NearbyWater.Num(),
        ScanDataset.NearbySpecies.Num()));

    if (ObservedManager->MigrationRoutes.IsValidIndex(ObservedManager->CurrentMigrationRouteIndex))
    {
        const FHerdMigrationRoute& Route = ObservedManager->MigrationRoutes[ObservedManager->CurrentMigrationRouteIndex];
        MigrationRouteText = FText::FromString(FString::Printf(TEXT("%s waypoint %d/%d"),
            *Route.RouteName.ToString(),
            ObservedManager->CurrentMigrationWaypointIndex + 1,
            Route.Waypoints.Num()));
    }
    else
    {
        MigrationRouteText = FText::FromString(TEXT("No active migration route"));
    }
}
