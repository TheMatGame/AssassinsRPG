// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"


// Sets default values
AWeapons::AWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataObject(TEXT("/Script/Engine.DataTable'/Game/EquipmentSystem/DT_Items.DT_Items'"));
	if (ItemDataObject.Succeeded())
	{
		ItemData.DataTable = ItemDataObject.Object;
	}
	
	
}

//para reconstruir el objeto en el editor
void AWeapons::OnConstruction(const FTransform& Transform)
{
	FEquipmentItems* Item = ItemData.GetRow<FEquipmentItems>(FString(TEXT("Item Context")));
	if (Item)
	{
		WeaponMesh->SetStaticMesh(Item->StaticMesh);
	}
	else UE_LOG(LogTemp, Warning, TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa-------------------------aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
}

// Called when the game starts or when spawned
void AWeapons::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

