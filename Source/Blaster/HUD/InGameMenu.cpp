// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

void UInGameMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	SetLeaveButtonFunctionality(true);

	if (SettingsButton && !SettingsButton->OnClicked.IsBound())
	{
		SettingsButton->OnClicked.AddDynamic(this, &UInGameMenu::SettingsButtonClicked);
	}
	TakeWidget();
}

void UInGameMenu::MenuTearDown()
{
	RemoveFromParent();

	SetLeaveButtonFunctionality(false);

	if (SettingsButton && !SettingsButton->OnClicked.IsBound())
	{
		SettingsButton->OnClicked.RemoveDynamic(this, &UInGameMenu::SettingsButtonClicked);
	}
}

void UInGameMenu::SetLeaveButtonFunctionality(bool bInMenu)
{
	if (bInMenu)
	{
		if (LeaveButton && !LeaveButton->OnClicked.IsBound())
		{
			LeaveButton->OnClicked.AddDynamic(this, &UInGameMenu::LeaveButtonClicked);
		}
		UGameInstance* GameInstance = GetGameInstance();
		if (GameInstance)
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
			if (MultiplayerSessionsSubsystem)
			{
				MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UInGameMenu::OnDestroySession);
			}
		}
	}
	else
	{
		if (LeaveButton && LeaveButton->OnClicked.IsBound())
		{
			LeaveButton->OnClicked.RemoveDynamic(this, &UInGameMenu::LeaveButtonClicked);
		}

		if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UInGameMenu::OnDestroySession);
		}
	}
}

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UInGameMenu::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		LeaveButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UInGameMenu::LeaveButtonClicked()
{
	LeaveButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* FirstPlayerController = World->GetFirstPlayerController();
		if (FirstPlayerController)
		{
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FirstPlayerController->GetPawn());
			if (BlasterCharacter)
			{
				BlasterCharacter->ServerLeaveGame();
				BlasterCharacter->OnLeftGame.AddDynamic(this, &UInGameMenu::OnPlayerLeftGame);
			}
			else
			{
				LeaveButton->SetIsEnabled(true);
			}
		}
	}
}

void UInGameMenu::SettingsButtonClicked()
{
	RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World)
	{
		ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(World->GetFirstPlayerController());
		if (BPlayerController)
		{
			BPlayerController->ShowInGameSettingsMenu();
		}
	}
}


void UInGameMenu::OnPlayerLeftGame()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}
