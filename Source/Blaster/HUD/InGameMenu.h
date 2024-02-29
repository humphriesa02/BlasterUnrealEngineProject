// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void MenuSetup();
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();
private:
	UPROPERTY()
	class APlayerController* PlayerController;

	void SetLeaveButtonFunctionality(bool bInMenu);

	/*
	* Leave button
	*/
	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveButton;

	UFUNCTION()
	void LeaveButtonClicked();

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	/*
	* Settings button
	*/
	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;

	UFUNCTION()
	void SettingsButtonClicked();
};
