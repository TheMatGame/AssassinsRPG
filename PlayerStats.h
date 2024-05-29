// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS( ClassGroup=(Custom),  meta = (BlueprintSpawnableComponent) )
class RPG_ASSASIN_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	bool bIsAlive = true;

	UPROPERTY()
	bool bHasStamina = true;

	//HEALTH
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	float MaxHealth = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	float CurrentHealth;

	//STAMINA
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stamina)
	float MaxStamina = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stamina)
	float CurrentStamina;

	//XP
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XP)
	float MaxXp = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XP)
	float CurrentXp;

	//LEVEL
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = XP)
	int Level = 1;

	UFUNCTION(BlueprintCallable)
	bool DecreaseHealth(float Damage);
	UFUNCTION(BlueprintCallable)
	void IncreaseCurrentHealth(float Health);
	UFUNCTION(BlueprintCallable)
	void IncreaseMaxHealth(float Health);

	UFUNCTION(BlueprintCallable)
	bool DecreaseStamina(float StaminaLost);
	UFUNCTION(BlueprintCallable)
	void IncreaseCurrentStamina(float StaminaIncrease);
	UFUNCTION(BlueprintCallable)
	void IncreaseMaxStamina(float StaminaIncrease);

	UFUNCTION(BlueprintCallable)
	void IncreaseXp(float XpIncrease);
	UFUNCTION(BlueprintCallable)
	void IncreaseLevel(int LevelIncrease);

	UPROPERTY(EditAnywhere)
	class USoundBase* LevelUpSound;



	class UPlayerHUD* HUD;
	class UDummyHealthBar* HealthBar;
	UFUNCTION(BlueprintCallable)
	void CallHUD();

};
