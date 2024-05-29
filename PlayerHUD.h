// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ASSASIN_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealth(float CurrentHealth, float MaxHealth);

	void SetStamina(float CurrentStamina, float MaxStamina);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* StaminaBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HPinfo;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* STinfo;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LVLinfo;

	void SetLevel(int CurrentLevel);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* XpBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* XPinfo;

	void SetXp(float CurrentXp, float MaxXp);

};
