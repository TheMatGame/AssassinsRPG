// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_AssasinCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

#include "CharacterMovementComponentAsync.h"
#include "MotionWarpingComponent.h"

#include "PlayerStats.h"

#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "AssassinPlayerController.h"

#include "TimerManager.h"

#include "Animation/AnimMontage.h"

#include "PlayerAttack.h"
#include "StealthBackAssassin.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "EquipmentSystem.h"



//////////////////////////////////////////////////////////////////////////
// ARPG_AssasinCharacter

ARPG_AssasinCharacter::ARPG_AssasinCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));

	SwordTopPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SwordTopPoint"));
	SwordBottomPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SwordBottomPoint"));
	KickPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("KickPoint"));
	SwordTopPoint->SetupAttachment(Sword);
	SwordBottomPoint->SetupAttachment(Sword);
	KickPoint->SetupAttachment(GetMesh());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//TIMELINE
	
	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchingTimeline"));
	ProgressUpdate.BindUFunction(this, FName("CrouchUpdate"));

	WarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("WarpingComponent"));

	Stats = CreateDefaultSubobject<UPlayerStats>(TEXT("PlayerStats"));

	Attack = CreateDefaultSubobject<UPlayerAttack>(TEXT("PlayerAttack"));

	


}

void ARPG_AssasinCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));


	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (CrouchingCurve)
	{
		MyTimeline->AddInterpFloat(CrouchingCurve, ProgressUpdate);
	}
	else UE_LOG(LogTemp, Fatal, TEXT("NO CROUCHING CURVE"));

	AAssassinPlayerController* PlayerController = GetController<AAssassinPlayerController>();
	HUDScreen = CreateWidget<UPlayerHUD>(PlayerController, HUDScreenClass);
    if (HUDScreen)
    {
        HUDScreen->AddToViewport();
		if (Stats)	Stats->CallHUD();
		else UE_LOG(LogTemp, Warning, TEXT("nullptr a statas"));
    }
	else UE_LOG(LogTemp, Error, TEXT("NO VIEWPORT"));

}

void ARPG_AssasinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSprinting)
	{
		Stats->IncreaseCurrentStamina(5.f*DeltaTime);
	}

	if (ActorToLock)
	{
		GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation() + FVector(0,0,100), ActorToLock->GetActorLocation()));
		
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_AssasinCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARPG_AssasinCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_AssasinCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_AssasinCharacter::Look);

		//Crouching 
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::Crouching);

		//Vault
		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::Vaulting);

		//Assassination
		EnhancedInputComponent->BindAction(AssassinAction, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::Assassination);

		//Sprint
		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::SprintHandler);
		EnhancedInputComponent->BindAction(Sprinting, ETriggerEvent::Completed, this, &ARPG_AssasinCharacter::SprintHandlerEnd);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::SwordTrigger);

		//LockEnemy
		EnhancedInputComponent->BindAction(LockEnemy, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::LookEnemy);
		
		//Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ARPG_AssasinCharacter::Dodge);


		//esto es feo hay que cambiarlo
		EquipmentSystem = FindComponentByClass<UEquipmentSystem>();
		EnhancedInputComponent->BindAction(EquipmentSystem->InteractAction, ETriggerEvent::Started, EquipmentSystem, &UEquipmentSystem::nombreinteraccion);
		EnhancedInputComponent->BindAction(EquipmentSystem->MenuAction, ETriggerEvent::Started, EquipmentSystem, &UEquipmentSystem::Menu);

	}

}

void ARPG_AssasinCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPG_AssasinCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPG_AssasinCharacter::Jump()
{
	if (bCrouching) 
	{
		Crouching();
	}
	ACharacter::Jump();
}

void ARPG_AssasinCharacter::Crouching()
{
	if (Controller != nullptr)
	{
		if(!bCrouching) 
		{
			bCrouching = true;
			IsCrouching();
		}
		else {
			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
			ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);


			FVector StartLocation = GetMesh()->GetComponentLocation();
			FVector EndLocation = GetMesh()->GetComponentLocation() + FVector(0,0,180);

			FHitResult HitResult;
			bool bCanDecrouch = !GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams);
			//DrawDebugLine(GetWorld(),StartLocation, EndLocation, FColor::Red, true);
			//DrawDebugSphere(GetWorld(), StartLocation, 10.f, 8, FColor::Purple, true);

			if (!bCanDecrouch) 
			{
				bCrouching = true;
			}
			else 
			{
				bCrouching = false;
				IsCrouching();
			}
		}
	}
}

void ARPG_AssasinCharacter::IsCrouching()
{
	if (bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
		MyTimeline->Play();
		GetCapsuleComponent()->SetCapsuleHalfHeight(60.f);
		GetMesh()->AddLocalOffset(FVector(0,0,30));
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		MyTimeline->Reverse();
		GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
		GetMesh()->AddLocalOffset(FVector(0,0,-30));
	}
}

void ARPG_AssasinCharacter::CrouchUpdate(float val)
{
	CameraTransition = FMath::Lerp(400.f,550.f,val);
	CameraBoom->TargetArmLength = CameraTransition;
	UE_LOG(LogTemp, Warning, TEXT("CameraTransition = %f"), CameraTransition);
}

//-----------------------------------------------------------------------------------------------------------------------------------------//

void ARPG_AssasinCharacter::Vaulting()
{	
	//Manera cutre de que no se bugee el juego cunado no podemos saltar una pared porque esta  muy alta pero si guarda la landigpos y entonces puedes saltar el aire
	VaultLandingPos = FVector(0,0,20000);
	bCanVault = false;
	// MIRAR QUE ES Y PARA QUE SIRVE Y SI SE ESTÁ USANDO BIEN
	FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorForwardVector() * 180.f;
	StartLocation.Z -= 30.f;

	//HACEMOS UN LINETRACE PARA VER LA ALTURA EN LA QUE SE ENCUENTRA EL OBJETO
	for (int i = 0; i < 3; i++)
	{	
		StartLocation.Z += 30.f;
		EndLocation = StartLocation + GetActorForwardVector() * 180.f;

		FHitResult OutHitResult;
		bool bHit = GetWorld()->SweepSingleByObjectType(
			OutHitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ObjectParams,
			FCollisionShape::MakeSphere(5.f)
		);

		if (bHit)
		{
			DrawDebugCylinder(GetWorld(), StartLocation, OutHitResult.Location, 5, 8, FColor::Green, false, 5.0f);
			//VAMOS A CALCULAR LA POSICION DE INICIO Y FINAL NECESARIA PARA SALTAR EL OBJETO
			VaultingLength(OutHitResult);
			break; //creo que un return serviria y quedaria mejor
		}
		else
		{
   	    	DrawDebugCylinder(GetWorld(), StartLocation, EndLocation, 5, 8, FColor::Red, false, 5.0f);
		}
	}
}

void ARPG_AssasinCharacter::VaultingLength(FHitResult &HitResult)
{
	//VAMOS A CALCULAR LA POSICION DE INICIO Y FINAL NECESARIA PARA SALTAR EL OBJETO
	for (int j = 0; j < 6; ++j)
	{
		FCollisionObjectQueryParams ObjectParams;
    	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
    	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

		FVector StartLocation = (HitResult.Location + FVector(0,0,100)) + (GetActorForwardVector() * 50.f * j);
		FVector EndLocation = StartLocation - FVector(0,0,100);

		FHitResult OutHitResult;
		bool bHit = GetWorld()->SweepSingleByObjectType(
			OutHitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ObjectParams,
			FCollisionShape::MakeSphere(7.f)
		);

		if (bHit)
		{
			DrawDebugCylinder(GetWorld(), OutHitResult.Location, EndLocation, 7, 8, FColor::Green, false, 5.0f);
			//Start
			if (j == 0)
			{
				DrawDebugSphere(GetWorld(), OutHitResult.Location, 7, 8, FColor::Purple, false, 5.f);
				VaultStartPos = OutHitResult.Location;
			}
			//Midlle part
			else
			{
				DrawDebugSphere(GetWorld(), OutHitResult.Location, 7, 8, FColor::Yellow, false, 5.f);
				VaultMiddlePos = OutHitResult.Location; 
				bCanVault = true;
			}
		}
		else
		{
			//CUANDO DEJEMOS DE GOLPEAR EL OBJETO ES MOMENTO DE CALCULAR LA POSICION FINAL
   	    	FVector StartLanding = OutHitResult.TraceStart + (GetActorForwardVector() * 80.f) + FVector(0,0,100);
			FVector EndLanding = StartLanding - FVector(0,0,1000);

			FHitResult LandingHitResult;
			bool bLanding = GetWorld()->LineTraceSingleByObjectType(
				LandingHitResult,
				StartLanding,
				EndLanding,
				ObjectParams
			);

			if (bLanding)
			{
				DrawDebugLine(GetWorld(), StartLanding, LandingHitResult.Location, FColor::Red, false, 5.0f);
				VaultLandingPos = LandingHitResult.Location;
				if (bCanVault) VaultingMovmentWarping();
				return;
			}
		}
	}
}

void ARPG_AssasinCharacter::VaultingMovmentWarping()
{
	if (bCanVault && InRange())
	{
		if (bCrouching) Crouching();
		bIsVaulting = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		SetActorEnableCollision(false);
		WarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("VaultStart"), VaultStartPos, GetActorRotation());
		WarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("VaultMiddle"), VaultMiddlePos, GetActorRotation());
		WarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("VaultEnd"), VaultLandingPos, GetActorRotation());
		//Creo que lo comprueba la propia función
		float OnCompleted = PlayAnimMontage(VaultMontage);
		if (OnCompleted != 0.f)
		{
			GetWorldTimerManager().SetTimer(VaultingTimer, this, &ARPG_AssasinCharacter::VaultingMovmentWarpingEnd, OnCompleted, false);
		}
	}
}

void ARPG_AssasinCharacter::VaultingMovmentWarpingEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorEnableCollision(true);
	bIsVaulting = false;
	VaultLandingPos += FVector(0,0,2000.f);
}

bool ARPG_AssasinCharacter::InRange()
{
	FVector BodyLocation = GetMesh()->GetComponentLocation();
	FVector BodyLocationLow = BodyLocation - FVector(0,0,50);
	FVector BodyLocationTop = BodyLocation + FVector(0,0,50);
	if (VaultLandingPos.Z <= BodyLocationTop.Z && VaultLandingPos.Z >= BodyLocationLow.Z) return true;
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------//

UPlayerHUD* ARPG_AssasinCharacter::GetPlayerHUD()
{
	return HUDScreen;
}

float ARPG_AssasinCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	bool bIsAlive = Stats->DecreaseHealth(DamageApplied);

	if (!bIsAlive)
	{
		//No podemos controlarlo
		DetachFromControllerPendingDestroy();
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	return DamageApplied;
}

void ARPG_AssasinCharacter::SprintHandler()
{
	if (Stats->CurrentStamina != 0 && !bCrouching && !bIsVaulting)
	{
		GetWorldTimerManager().SetTimer(TimerHandler, this, &ARPG_AssasinCharacter::Sprint, 0.2, true);
		GetCharacterMovement()->MaxWalkSpeed = 750.f;
	}
}

void ARPG_AssasinCharacter::SprintHandlerEnd()
{
	GetWorldTimerManager().ClearTimer(TimerHandler);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	
}

void ARPG_AssasinCharacter::Sprint()
{
	if (GetVelocity().Size() > 0.f)
	{
		bSprinting = Stats->DecreaseStamina(1.f);
	}
	else bSprinting = false;

	if (!bSprinting || bCrouching || bIsVaulting)
	{
		SprintHandlerEnd();
	}
}

void ARPG_AssasinCharacter::Assassination()
{
	TSet<AActor*> OutOverlappingActors;
	GetOverlappingActors(OutOverlappingActors);
	for (AActor* ActorOverlapping : OutOverlappingActors)
	{
		IStealthBackAssassin* Interface = Cast<IStealthBackAssassin>(ActorOverlapping);
		if (Interface)
		{
			FVector RefLoc;
			FRotator RefRot;
			Interface->StealthBackAssassin(RefLoc, RefRot);

			PlayAnimMontage(KnifeAnim);
			WarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("AssassinationWarp"), RefLoc, RefRot);
		}
	}
}

void ARPG_AssasinCharacter::SwordTrigger()
{
	Attack->SwordTrigger();
}

FVector ARPG_AssasinCharacter::GetSwordTopPoint()
{
	return SwordTopPoint->GetComponentLocation();
}

FVector ARPG_AssasinCharacter::GetSwordBottomPoint()
{
	return SwordBottomPoint->GetComponentLocation();
}

FVector ARPG_AssasinCharacter::GetKickPoint()
{
	return KickPoint->GetComponentLocation();
}

void ARPG_AssasinCharacter::LookEnemy()
{
	if (!ActorToLock)
	{
		FVector EndLocation = GetActorLocation() + FVector(1000) * GetActorForwardVector();

		TArray<AActor*> ActorsToIngore;
		TArray<FHitResult> Hit;
		UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), GetActorLocation(), EndLocation, 50.f, FName(TEXT("Pawn")), false, ActorsToIngore, EDrawDebugTrace::ForDuration, Hit, true);

		for (FHitResult HitActor : Hit)
		{
			if (HitActor.GetActor()->ActorHasTag(FName("Damageable"))) 
			{
				ActorToLock = HitActor.GetActor();
				return;
			}
		}
	}
	else
	{
		ActorToLock = nullptr;
	}

}

void ARPG_AssasinCharacter::Dodge()
{
	if (!bIsDoding)
	{
		if (Attack->bIsAttacking)
		{
			Attack->StopCombo();
		}
		float Playtime = PlayAnimMontage(DodgeAnim);
		GetWorldTimerManager().SetTimer(DodgeHandle, this, &ARPG_AssasinCharacter::Dodging, Playtime, false);
		bIsDoding = true;
	}
}

void ARPG_AssasinCharacter::Dodging()
{
	bIsDoding = false;
}











