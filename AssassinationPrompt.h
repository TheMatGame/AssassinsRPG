// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssassinationPrompt.generated.h"


/**
 * 
 */
UCLASS()
class RPG_ASSASIN_API UAssassinationPrompt : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Prompt;

	void SetPromptRotation(FRotator rotation);

	FVector GetPromptLocation();

};
