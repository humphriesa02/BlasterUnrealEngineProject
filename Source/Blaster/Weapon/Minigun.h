// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Minigun.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AMinigun : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitTarget) override;
	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Weapon Spin")
	float MaxSpinupTime = 2.f;

	float SpinupTime;

	UPROPERTY(EditAnywhere, Category = "Weapon Spin")
	float CooldownSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Spin")
	class UAnimationAsset* SpinAnimation;

	void SpinGun();

	void ResetSpinupTime();

	FTimerHandle SpinTimerHandle;
};
