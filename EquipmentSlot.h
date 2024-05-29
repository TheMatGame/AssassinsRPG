// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ASSASIN_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;

		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Button;
	
		UFUNCTION()
		void ButtonClicked();

};
