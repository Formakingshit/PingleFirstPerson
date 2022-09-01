// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PingleFirstPersonHUD.generated.h"

UCLASS()
class APingleFirstPersonHUD : public AHUD
{
	GENERATED_BODY()

public:
	APingleFirstPersonHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

