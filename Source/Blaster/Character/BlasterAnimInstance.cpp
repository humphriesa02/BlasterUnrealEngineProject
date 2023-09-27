// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called at the beginning of play
void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

// Called every frame
void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Ensure we have the blaster character before we do anything
	if (BlasterCharacter == nullptr) {
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}

	// If we still don't have a blaster character, it doesn't exist and return
	if (BlasterCharacter == nullptr) return;

	// Get velocity from the player
	FVector Velocity = BlasterCharacter->GetVelocity();
	// Zero out the Z velocity (the jump speed)
	Velocity.Z = 0.f;
	// Assign our local speed variable
	Speed = Velocity.Size();

	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}
