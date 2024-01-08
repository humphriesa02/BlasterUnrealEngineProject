// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Blaster/BlasterComponents/CombatComponent.h"


void AMinigun::BeginPlay()
{
	Super::BeginPlay();
	SpinupTime = MaxSpinupTime;
}

void AMinigun::Fire(const FVector& HitTarget)
{
	GetWorldTimerManager().ClearTimer(SpinTimerHandle);
	if (SpinupTime > 0)
	{
		SpinGun();
	}
	else
	{
		AWeapon::Fire(HitTarget);
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn == nullptr) return;
		AController* InstigatorController = OwnerPawn->GetController();
		BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;

		const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
		if (MuzzleFlashSocket)
		{
			FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
			FVector Start = SocketTransform.GetLocation();

			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if (BlasterCharacter && HasAuthority() && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					BlasterCharacter,
					Damage,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
			if (BlasterCharacter && PlayerImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					PlayerImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}
			else if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint
				);
			}
			GetWorldTimerManager().SetTimer(SpinTimerHandle, this, &AMinigun::ResetSpinupTime, CooldownSpeed, true);
		}
	}
}

void AMinigun::SpinGun()
{
	// Play spin animation
	if (SpinAnimation)
	{
		GetWeaponMesh()->PlayAnimation(SpinAnimation, false);
	}
	SpinupTime -= 1.f;
}

void AMinigun::ResetSpinupTime()
{
	SpinupTime = MaxSpinupTime;
}
