// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GateSDKHUD.generated.h"

UCLASS()
class AGateSDKHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGateSDKHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

