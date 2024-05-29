// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentUI.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ASSASIN_API UEquipmentUI : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;

		//text -> equipment
		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* EquipmentText;

		//image -> character
		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* CharacterImage;

		/*/vertical box_left
		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* BoxLeft;
		//vertical box_rightç
		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* BoxLeft;*/
public:
		UPROPERTY(meta = (BindWidget))
			class UCanvasPanel* ItemCanvas;

		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* ItemList;

		UPROPERTY(EditAnywhere)
		TSubclassOf<class UItemSlot> ItemSlotClass;
		//seguramente necesite un array para saber a cual quiero acceder
		UItemSlot* Slot;
};
