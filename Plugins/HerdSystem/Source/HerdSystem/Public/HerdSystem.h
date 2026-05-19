#pragma once

#include "Modules/ModuleManager.h"

class FHerdSystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
