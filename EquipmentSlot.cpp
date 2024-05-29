// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlot.h"
#include "Components/Button.h"
#include "RPG_AssasinCharacter.h"
#include "EquipmentSystem.h"
#include "EquipmentUI.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UEquipmentSlot::ButtonClicked);
}

void UEquipmentSlot::ButtonClicked()
{
	ARPG_AssasinCharacter* Player = Cast<ARPG_AssasinCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	

	if (Player)
	{
		UCanvasPanel* Items = Player->EquipmentSystem->MenuUI->ItemCanvas;
		Items->SetVisibility(ESlateVisibility::Visible);

		float x, y;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(x, y);
		FVector2D mouse = { x,y };
		Items->SetRenderTransform(FWidgetTransform(mouse,FVector2D(1),FVector2D::ZeroVector,0.f));

	}
}