// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "BlasterPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void UpdateHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void SetHUDWeaponType(EWeaponType CurrentWeaponType, bool bIsVisible);
	void SetHUDElimmedText(bool isShown);
	void SetHUDGrenades(int32 Grenades);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void HideTeamScores();
	void InitTeamScores();
	void SetHUDRedTeamScore(int32 RedScore);
	void SetHUDBlueTeamScore(int32 BlueScore);

	virtual float GetServerTime(); // Synced with server world clock

	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

	void OnMatchStateSet(FName State, bool bTeamsMatch = false);
	void HandleMatchHasStarted(bool bTeamsMatch = false);
	void HandleMatchCooldown();

	float SingleTripTime = 0.f;

	FHighPingDelegate HighPingDelegate;

	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);

	bool bChatBoxOpen = false;

	void BroadCastChatMessage(const FString& MessageText, const FString& UserName);

	UFUNCTION(Server, Reliable)
	void AskServerGameModeToDisplayMessage(const FString& Message, const FString& UserName);

	void ShowInGameSettingsMenu();

	UFUNCTION(BlueprintCallable)
	void ShowInGameMenu();
protected:
	virtual void BeginPlay() override;
	void SetHUDTime();
	void PollInit();

	virtual void SetupInputComponent() override;

	/**
	* Sync time between client and server
	*/
	// CLIENT -> SERVER
	// Client requests the current server time, passing in the client's time when the request was sent.
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// SERVER -> CLIENT
	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	// Difference between client and server time
	float ClientServerDelta = 0.f;

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);

	// Handles relaying Game mode information to the server
	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	// Handles relaying game mode information when a client joins the game
	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaTime);
	void DisplayPing();

	void OpenMenus();

	void ShowChatBox();

	UFUNCTION(Client, Reliable)
	void ClientElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

	UFUNCTION(Client, Reliable)
	void ClientChatMessage(const FString& Message, const FString& UserName);

	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
	bool bShowTeamScores = false;

	UFUNCTION()
	void OnRep_ShowTeamScores();

	FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players);
	FString GetTeamsInfoText(class ABlasterGameState* BlasterGameState);
private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	/*
	* Return to main menu
	*/

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> InGameMenuWidget;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> ChatBoxWidget;
	
	UPROPERTY()
	class UInGameMenu* InGameMenu;

	UPROPERTY()
	class UChatBox* ChatBox;

	bool bInGameMenuOpen = false;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	float HUDHealth;
	bool bInitializeHealth = false;
	float HUDMaxHealth;
	float HUDScore;
	bool bInitializeScore = false;
	int32 HUDDefeats;
	bool bInitializeDefeats = false;
	int32 HUDGrenades;
	bool bInitializeGrenades = false;
	float HUDShield;
	bool bInitializeShield = false;
	float HUDMaxShield;
	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;
	EWeaponType HUDWeaponType;
	bool bInitializeWeaponTypeName = false;

	/*
	* High Ping
	*/

	float HighPingRunningTime = 0.f;

	// How long we display the high ping warning
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.0f;

	float PingAnimationRunningTime = 0.f;

	// How often we check for high ping
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);

	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 60.f;

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
