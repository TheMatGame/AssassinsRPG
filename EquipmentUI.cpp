// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "RPG_AssasinCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EquipmentSystem.h"
#include "AssassinPlayerController.h"
#include "ItemSlot.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Texture2D.h"



void UEquipmentUI::NativeConstruct()
{
	ARPG_AssasinCharacter* Player = Cast<ARPG_AssasinCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AAssassinPlayerController* PlayerController = Player->GetController<AAssassinPlayerController>();

	for (FEquipmentItems Elements : Player->EquipmentSystem->WeaponSlots)
	{
		Slot = CreateWidget<UItemSlot>(PlayerController, ItemSlotClass);
		ItemList->AddChildToVerticalBox(Slot);

		Slot->Icon->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(Elements.Icon, 75, 75));
		Slot->Text->SetText(FText::FromString(Elements.Name));
	}
}