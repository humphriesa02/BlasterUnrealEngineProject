// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blaster/Weapon/WeaponTypes.h"
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
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDWeaponType(EWeaponType CurrentWeaponType, bool bIsVisible);
	void SetHUDElimmedText(bool isShown);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	void SetHUDTime();
private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	float MatchTime = 120.f;
	uint32 CountdownInt = 0;

	TArray<FString> ElimmedTextArray = {
		TEXT("Get PWNED, Nerd!"),
		TEXT("Have you tried shooting back?"),
		TEXT("Oooooooooooof."),
		TEXT("Alt + F4."),
		TEXT("Don't Hate the Player..."),
		TEXT("Really?"),
		TEXT("You've lost, like, a lot of blood.")
	};
};
