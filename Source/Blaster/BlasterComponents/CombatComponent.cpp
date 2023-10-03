// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	// Set aiming locally, so the caller instantly sees the change
	bAiming = bIsAiming;
	// Then set aiming by replication so all others see the change
	ServerSetAiming(bIsAiming);
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Rep weapon equipped"));
	if (EquippedWeapon && Character) {
		UE_LOG(LogTemp, Warning, TEXT("Equipped weapon and character valid"));
		// When we equip a weapon, have the player always facing the camera direction
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Replicate the equipped weapon
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon)
	DOREPLIFETIME(UCombatComponent, bAiming)
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) {
		return;
	}

	// Set the local variable weapon to be whatever we are equipping
	EquippedWeapon = WeaponToEquip;
	// Set that local weapon's state to be equipped
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	// Get the skeletal mesh socket of the right hand attached to the character
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	// Attach the equipped weapon mesh to the character's hand socket
	if (HandSocket) {
		HandSocket->AttachActor(EquippedWeapon, Character-> GetMesh());
	}
	// Sets the owner of the weapon to be character, used for replication
	EquippedWeapon->SetOwner(Character);

	// When we equip a weapon, have the player always facing the camera direction
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

