// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatMessage.h"
#include "Components/TextBlock.h"


void UChatMessage::SetChatUserName(FString UserName)
{
	FString ChatUserName = FString::Printf(TEXT("%s:"), *UserName);

	if (ChatUserNameText)
	{
		ChatUserNameText->SetText(FText::FromString(ChatUserName));
	}
}

void UChatMessage::SetChatMessageText(FString Message)
{
	if (ChatMessageText)
	{
		ChatMessageText->SetText(FText::FromString(*Message));
	}
}
