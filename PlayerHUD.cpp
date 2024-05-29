// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"


void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(CurrentHealth/MaxHealth);
        if (HPinfo)
        {
            //Vaya lio para poner un texto
            FText aux = FText::FromString(TEXT("HP: ") + FString::Printf(TEXT("%d"), int(CurrentHealth)) + TEXT("/") + FString::Printf(TEXT("%d"), int(MaxHealth)));
            HPinfo->SetText(aux);
        }
    }
}

void UPlayerHUD::SetStamina(float CurrentStamina, float MaxStamina)
{
    if (StaminaBar)
    {
        StaminaBar->SetPercent(CurrentStamina/MaxStamina);
        FText aux = FText::FromString(TEXT("Stamina: ") + FString::Printf(TEXT("%d"), int(CurrentStamina)) + TEXT("/") + FString::Printf(TEXT("%d"), int(MaxStamina)));
        STinfo->SetText(aux);
    }
}

void UPlayerHUD::SetLevel(int CurrentLevel)
{
    if (LVLinfo)
    {
        FText aux = FText::FromString(TEXT("Lvl: ") + FString::Printf(TEXT("%i"), CurrentLevel));
        LVLinfo->SetText(aux);
    }
}

void UPlayerHUD::SetXp(float CurrentXp, float MaxXp)
{
    if (XpBar)
    {
        XpBar->SetPercent(CurrentXp/MaxXp);
        FText aux = FText::FromString(FString::Printf(TEXT("%d"), int(CurrentXp)) + TEXT(" / ") + FString::Printf(TEXT("%d"), int(MaxXp)));
        XPinfo->SetText(aux);
    }
}