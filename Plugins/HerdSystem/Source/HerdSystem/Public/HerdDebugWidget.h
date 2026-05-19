#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HerdTypes.h"
#include "HerdDebugWidget.generated.h"

class AHerdManager_Generic;

UCLASS(BlueprintType, Blueprintable)
class HERDSYSTEM_API UHerdDebugWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    TObjectPtr<AHerdManager_Generic> ObservedManager = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (ClampMin = "0.0"))
    float RefreshInterval = 0.25f;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    TArray<FHerdDebugMemberRow> MemberRows;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    FText HerdCompositionText;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    FText ThreatSummaryText;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    FText MigrationRouteText;

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void SetObservedManager(AHerdManager_Generic* InManager);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void Refresh();

private:
    float RefreshTimer = 0.0f;
};
