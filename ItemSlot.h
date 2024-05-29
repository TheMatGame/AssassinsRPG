// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ASSASIN_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
		UPROPERTY(meta = (BindWidget))
		class UButton* Button;
	
		UPROPERTY(meta = (BindWidget))
		class UImage* Icon;
		
		UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text;
};
