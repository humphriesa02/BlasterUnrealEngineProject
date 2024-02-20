// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessage.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetChatUserName(FString UserName);

	void SetChatMessageText(FString Message);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatUserNameText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* ChatBoxContainer;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ChatMessageText;
};
