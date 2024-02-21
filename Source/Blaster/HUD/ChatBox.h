// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBox.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UChatBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void MenuSetup();
	void MenuTearDown();

private:
	UFUNCTION()
	void ChatMessageSubmitted(const FText& Text, ETextCommit::Type CommitMethod);

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* ChatBoxContainer;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatTextBox;

	UPROPERTY()
	class APlayerController* PlayerController;
	
	UFUNCTION()
	bool ValidateText(FString CommittedText);

	bool HasExplicitWords(const FString& Message);

	int32 MaxMessageLength = 50;

	UFUNCTION()
	FString ReplaceWord(const FString& OriginalMessage, const FString& WordToReplace, const FString& ReplacementWord);
};
