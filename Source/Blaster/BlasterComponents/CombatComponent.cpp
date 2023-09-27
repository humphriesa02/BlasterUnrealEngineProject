// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
}

