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

	ABlasterPlayerController* FirstPlayerController = Cast<ABlasterPlayerController>(GetOwningPlayer());

	// Determine the commit method and handle accordingly
	if (FirstPlayerController)
	{
		APlayerState* PlayerState = FirstPlayerController->PlayerState;
		if (PlayerState)
		{
			FString PlayerName = PlayerState->GetPlayerName();
			switch (CommitMethod)
			{
			case ETextCommit::OnEnter:
				if (!CommittedText.IsEmpty())
				{
					if (ValidateText(CommittedText))
					{
						FirstPlayerController->AskServerGameModeToDisplayMessage(CommittedText, PlayerName);
					}
					else
					{
						FirstPlayerController->AskServerGameModeToDisplayMessage(FString("I tried to write invalid text!"), PlayerName);
					}
				}
				else
				{
					FirstPlayerController->bChatBoxOpen = false;
					MenuTearDown();
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

bool UChatBox::ValidateText(FString CommittedText)
{
	if (CommittedText.Len() > MaxMessageLength)
	{
		return false;
	}

	for (TCHAR Char : CommittedText)
	{
		if (!FChar::IsAlnum(Char) && Char != ' ')
		{
			return false;
		}
	}

	if (CommittedText.Contains("Dillon"))
	{
		CommittedText = ReplaceWord(CommittedText, "Dillon", "Loser");
	}

	if (HasExplicitWords(CommittedText))
	{
		return false;
	}

	return true;
}

bool UChatBox::HasExplicitWords(FString Message)
{
	TArray<FString> BadWords = { "ez", "Easy", "easy" };

	for (const FString& BadWord : BadWords)
	{
		if (Message.Contains(BadWord, ESearchCase::IgnoreCase))
		{
			return true;
		}
	}

	return false;
}

FString UChatBox::ReplaceWord(const FString& OriginalMessage, const FString& WordToReplace, const FString& ReplacementWord)
{
	// Make a copy of the original message to perform the replacement
	FString ModifiedMessage = OriginalMessage;

	// Replace the word if it exists in the message
	ModifiedMessage.ReplaceInline(*WordToReplace, *ReplacementWord, ESearchCase::IgnoreCase);

	return ModifiedMessage;
}
