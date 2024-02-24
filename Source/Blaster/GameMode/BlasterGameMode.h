// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

// Custom match states for the Blaster Game mode to be in.
namespace MatchState
{
	extern BLASTER_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer.
}

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABlasterGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, class ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);
	void ServerDisplayMessageOnAllScreens(const FString& Message);
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	// Time it takes to warm up, before game begins
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	// Time limit for each match
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	// Duration of cooldown between matches
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	// The time when the GameMode is initialized (not the game itself)
	float LevelStartingTime = 0.f;

	bool bTeamsMatch = false;

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
private:
	float CountdownTime = 0.f;
public:
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
