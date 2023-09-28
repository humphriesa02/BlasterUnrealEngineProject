// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/BlasterComponents/CombatComponent.h"

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom for our camera, and set up it's settings
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// Attaching to mesh instead of root to prevent camera boom movement when root capsule size changes
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create and set up the actual camera object
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Prevent character rotation with mouse movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Name of player gets displayed over their head with this component
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// General combat component to be replicated
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	// Ability to crouch is always true
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// Binds the input mapping "MoveForward" to the function in this file called "MoveForward"
	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);
	
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABlasterCharacter::CrouchButtonPressed);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABlasterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABlasterCharacter::AimButtonReleased);
}

// Set up our connection to components
void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// If combat has been initialized, set it's character to this class
	if (Combat) {
		Combat->Character = this;
	}
}

void ABlasterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f) {

		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f) {

		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABlasterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABlasterCharacter::EquipButtonPressed()
{
	if (Combat) {
		// If on the server equip the weapon directly
		if (HasAuthority()) {
			Combat->EquipWeapon(OverlappingWeapon);
		}
		// If not the server then call the RPC in order to tell the server we are equipping
		else {
			ServerEquipButtonPressed();
		}
	}
}

// Client's replicated call to equip button
void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat) {
		Combat->EquipWeapon(OverlappingWeapon);
	}
}


void ABlasterCharacter::CrouchButtonPressed()
{
	if (bIsCrouched) {
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void ABlasterCharacter::AimButtonPressed()
{
	if (Combat) {
		Combat->bAiming = true;
	}
}

void ABlasterCharacter::AimButtonReleased()
{
	if (Combat) {
		Combat->bAiming = false;
	}
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	// Are we the server?
	if (IsLocallyControlled()) {
		if (OverlappingWeapon) {
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon) {
		LastWeapon->ShowPickupWidget(false);
	}
}

// Public weapon equipped getter
bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

// Public aiming getter
bool ABlasterCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

