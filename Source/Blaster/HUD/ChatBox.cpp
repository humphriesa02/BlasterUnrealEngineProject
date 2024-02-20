// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatBox.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Components/EditableTextBox.h"
#include "Types/SlateEnums.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/GameMode/BlasterGameMode.h"

void UChatBox::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if (ChatTextBox)
	{
		ChatTextBox->SetUserFocus(PlayerController);
	}

	if (ChatTextBox && !ChatTextBox->OnTextCommitted.IsBound())
	{
		ChatTextBox->OnTextCommitted.AddDynamic(this, &UChatBox::ChatMessageSubmitted);
	}
}

void UChatBox::MenuTearDown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	if (ChatTextBox && ChatTextBox->OnTextCommitted.IsBound())
	{
		ChatTextBox->OnTextCommitted.RemoveDynamic(this, &UChatBox::ChatMessageSubmitted);
	}

}

void UChatBox::ChatMessageSubmitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Your implementation here
	FString CommittedText = Text.ToString();


	UWorld* World = GetWorld();
	if (World)
	{
		ABlasterPlayerController* FirstPlayerController = Cast<ABlasterPlayerController>(GetOwningPlayer());

		// Determine the commit method and handle accordingly
		if (FirstPlayerController)
		{
			switch (CommitMethod)
			{
			case ETextCommit::OnEnter:
				for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
				{
					ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
					if (BlasterPlayer)
					{
						UE_LOG(LogTemp, Warning, TEXT("Chat iterated over"));
						BlasterPlayer->BroadCastChatMessage(CommittedText);
					}
				}
				break;
			case ETextCommit::OnUserMovedFocus:
				FirstPlayerController->bChatBoxOpen = false;
				MenuTearDown();
				break;
			case ETextCommit::OnCleared:
				FirstPlayerController->bChatBoxOpen = false;
				MenuTearDown();
				break;
			}
		}
	}
	ChatTextBox->SetText(FText::FromString(TEXT("")));
}