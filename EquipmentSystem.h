// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons.h"
#include "EquipmentSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_ASSASIN_API UEquipmentSystem : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UEquipmentSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FEquipmentItems> WeaponSlots;

	void AddWeapon(FEquipmentItems& NewItem);

	void nombreinteraccion();

	void Menu();

	bool bIsPanelOpened = false;
	class UEquipmentUI* MenuUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEquipmentUI> MenuUIClass;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Menu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MenuAction;
		
};
