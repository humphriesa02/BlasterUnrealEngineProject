// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDElimmedText(bool isShown);
	virtual void OnPossess(APawn* InPawn) override;
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	TArray<FString> ElimmedTextArray = {
		TEXT("Get PWNED, Nerd!"),
		TEXT("Have you tried shooting back?"),
		TEXT("Oooooooooooof."),
		TEXT("Alt + F4."),
		TEXT("Don't Hate the Player..."),
		TEXT("Really?")
	};
};
