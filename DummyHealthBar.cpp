// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyHealthBar.h"
#include "Components/ProgressBar.h"


void UDummyHealthBar::SetHealth(float CurrentHealth, float MaxHealth)
{
    if (HealthBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("-----------------------------------------%f"), CurrentHealth/MaxHealth);
        HealthBar->SetPercent(CurrentHealth/MaxHealth);
    }
}