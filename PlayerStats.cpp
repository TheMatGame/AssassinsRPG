// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"
#include "RPG_AssasinCharacter.h"
#include "BP_Dummy.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.h"
#include "AssassinPlayerController.h"
#include "GameFramework/Controller.h"

#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "DummyHealthBar.h"


// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	CurrentXp = 0;
}


// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayerStats::DecreaseHealth(float Damage)
{
	CurrentHealth -= FMath::Min(Damage, CurrentHealth);
	if (CurrentHealth == 0) bIsAlive = false;
	else bIsAlive = true;
	//CurrentHealth = (CurrentHelath == 0 ? false : true);
	if(HUD)
	{
		HUD->SetHealth(CurrentHealth, MaxHealth);
	}
	else if (HealthBar)
	{
		HealthBar->SetHealth(CurrentHealth, MaxHealth);
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f, Current Health: %f"), Damage, CurrentHealth);
	}
	
	return bIsAlive;
}

void UPlayerStats::IncreaseCurrentHealth(float Health)
{	
	//Lo que me puedo llegar a curar sin pasarme del maximo
	float HealthRange = MaxHealth - CurrentHealth;
	CurrentHealth += FMath::Min(Health, HealthRange);
	if(HUD)
	{
		HUD->SetHealth(CurrentHealth, MaxHealth);
	}
	else if (HealthBar)
	{
		HealthBar->SetHealth(CurrentHealth, MaxHealth);
	}
}

void UPlayerStats::IncreaseMaxHealth(float Health)
{	
	MaxHealth += Health;
	if(HUD)
	{
		HUD->SetHealth(CurrentHealth, MaxHealth);
	}
	else if (HealthBar)
	{
		HealthBar->SetHealth(CurrentHealth, MaxHealth);
	}
}

bool UPlayerStats::DecreaseStamina(float StaminaLost)
{
	CurrentStamina -= FMath::Min(StaminaLost, CurrentStamina);
	if (CurrentStamina == 0) bHasStamina = false;
	else bHasStamina = true; 
	HUD->SetStamina(CurrentStamina, MaxStamina);

	return bHasStamina;
}

void UPlayerStats::IncreaseCurrentStamina(float StaminaIncrease)
{	
	//Lo que me puedo llegar a curar sin pasarme del maximo
	float StaminaRange = MaxStamina - CurrentStamina;
	CurrentStamina += FMath::Min(StaminaIncrease, StaminaRange);
	HUD->SetStamina(CurrentStamina, MaxStamina);
}

void UPlayerStats::IncreaseMaxStamina(float StaminaIncrease)
{	
	MaxStamina += StaminaIncrease;
	HUD->SetStamina(CurrentStamina, MaxStamina);
}

void UPlayerStats::IncreaseXp(float XpIncrease)
{
	CurrentXp += XpIncrease;

	//Yo aqui ponia un while por si sube mucha xp por lo tanto muchos niveles de golpe O hacer algun calcula para saber cuanto subir;
	if (CurrentXp >= MaxXp)
	{
		CurrentXp -= MaxXp; 
		IncreaseLevel(1);
	}

	HUD->SetXp(CurrentXp, MaxXp);
}

void UPlayerStats::IncreaseLevel(int LevelIncrease)
{
	Level += LevelIncrease;
	//esto se cambiara por un vector o un map con cuanto tiene que subir en cada nivel.
	MaxXp += 150.f;
	HUD->SetLevel(Level);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LevelUpSound, GetOwner()->GetActorLocation());

}




void UPlayerStats::CallHUD()
{

	ARPG_AssasinCharacter* IsPlayer = GetOwner<ARPG_AssasinCharacter>();
	if (IsPlayer)
	{
		HUD = IsPlayer->GetPlayerHUD();
		if (HUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("si HUD"));
			IncreaseCurrentHealth(0);
			IncreaseCurrentStamina(0);
			IncreaseXp(0);
			IncreaseLevel(0);
		}
	}
	else{
		ABP_Dummy* IsDummy = GetOwner<ABP_Dummy>();

		if (IsDummy)
		{
			HealthBar = IsDummy->GetHealthBar();

			if (HealthBar)
			{
				UE_LOG(LogTemp, Warning, TEXT("Health Bar"));
				IncreaseCurrentHealth(100);
			}
		}
	}

}