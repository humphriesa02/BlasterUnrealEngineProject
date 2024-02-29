// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Determine what gets drawn to the screen here
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	void AddCharacterOverlay();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<class UUserWidget> AnnouncementClass;
	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);
	void AddChatMessage(const FString& MessageText, const FString& UserName="Error");

	UPROPERTY()
	class UAnnouncement* Announcement;

	/**
	* Handle in game menu here
	*/
	void ToggleInGameMenu();

	void ToggleSettingsMenu();

	// Overall, is there some menu open
	bool bMenuIsOpen = false;

	void CloseAllMenus();
protected:
	virtual void BeginPlay() override;
private:

	UPROPERTY()
	class APlayerController* OwningPlayer;

	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChatMessage> ChatMessageClass;

	UPROPERTY(EditAnywhere)
	float ChatDisplayTime = 2.5f;

	UFUNCTION()
	void ChatTimerFinished(UChatMessage* MsgToRemove);

	UPROPERTY()
	TArray<UChatMessage*> ChatMessages;

	void SetPlayerFocusOnWidget(TSharedRef<SWidget>);

	void ResetPlayerFocus();

	/**
	* In Game Menu base
	*/

	UPROPERTY(EditAnywhere, Category = InGameMenu)
	TSubclassOf<class UUserWidget> InGameMenuWidget;

	UPROPERTY()
	class UInGameMenu* InGameMenu;

	bool bInGameMenuOpen = false;


	/*
	* Settings Menu
	*/
	UPROPERTY(EditAnywhere, Category = InGameMenu)
	TSubclassOf<class UUserWidget> SettingsMenuWidget;

	UPROPERTY()
	class UUserWidget* SettingsMenu;

	bool bSettingsMenuOpen = false;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
