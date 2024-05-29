// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Weapons.generated.h"

UENUM()
enum class EWeaponType : uint8
{
	Mele,
	Range,
	Armor,
	Shields,	
};

USTRUCT(BlueprintType)
struct FEquipmentItems : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int RequiredLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EWeaponType Type;
};

//No se si este es el sitio mas indicado

UCLASS()
class RPG_ASSASIN_API AWeapons : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapons();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Construction Script c++
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category ="Data")
		FDataTableRowHandle ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category ="Data")
		class UUserDefinedStruct* Slots;

};
