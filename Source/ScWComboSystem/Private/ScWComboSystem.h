// Scientific Ways

#pragma once

#include "CoreMinimal.h"

#include "Kismet/GameplayStatics.h"

class FScWComboSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
