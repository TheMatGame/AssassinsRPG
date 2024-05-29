// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem.h"
#include "InputAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "RPG_AssasinCharacter.h"
#include "Weapons.h"
#include "EquipmentUI.h"
#include "AssassinPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UEquipmentSystem::UEquipmentSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> IAObject(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact'"));
	if (IAObject.Succeeded())
	{
		InteractAction = IAObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IMenuObject(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Menu.IA_Menu'"));
	if (IMenuObject.Succeeded())
	{
		MenuAction = IMenuObject.Object;
	}


}


// Called when the game starts
void UEquipmentSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}


// Called every frame
void UEquipmentSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UEquipmentSystem::AddWeapon(FEquipmentItems& NewItem)
{
	WeaponSlots.Add(NewItem);
}


void UEquipmentSystem::nombreinteraccion()
{
	ARPG_AssasinCharacter* Character = Cast<ARPG_AssasinCharacter>(GetOwner());
	UCameraComponent* CharacterCamera = Character->GetFollowCamera();

	FVector StartLocation = CharacterCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + CharacterCamera->GetForwardVector() * 1000;
	FHitResult OutHit;
	//TArray<> ObjectTypes;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	bool bHit =UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, 15.f, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true);

	if (bHit)
	{
		AWeapons* Weapon = Cast<AWeapons>(OutHit.GetActor());
		FEquipmentItems* Item = Weapon->ItemData.GetRow<FEquipmentItems>(FString(TEXT("Item Context")));
		AddWeapon(*Item);
		if (Weapon)
		{
			Weapon->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("OBJETO RECOGIDO"));
		}
	}
}


void UEquipmentSystem::Menu()
{
	ARPG_AssasinCharacter* Player = Cast<ARPG_AssasinCharacter>(GetOwner());
	AAssassinPlayerController* PlayerController = Player->GetController<AAssassinPlayerController>();

	if (!bIsPanelOpened)
	{
		MenuUI = CreateWidget<UEquipmentUI>(PlayerController, MenuUIClass);
		if (MenuUI)
		{
			MenuUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Menu Abierto"));

			PlayerController->bShowMouseCursor = true;

			//FOCUS AL UI PARA QUE EL RATON SOLO INTERACTUE CON ESTA
			PlayerController->SetInputMode(FInputModeGameAndUI());

			//Quitar movimiento al player
			Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}
	}
	else
	{
		if (MenuUI)
		{
			MenuUI->RemoveFromParent();

			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());

			Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		}
	}
	bIsPanelOpened = !bIsPanelOpened;
}
