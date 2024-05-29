// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RPG_AssasinCharacter.generated.h"

class UInputAction;

UCLASS(config=Game)
class ARPG_AssasinCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Sword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SwordTopPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SwordBottomPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* KickPoint;
	


		/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Vault Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VaultAction;

	/** Assassination Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AssassinAction;

	/** Vault Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Sprinting;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	/** LockEnemy Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockEnemy;
	
	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	

public:
	ARPG_AssasinCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Jump();

	/** Called for looking input */
	void Crouching();
	void IsCrouching();
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCrouching = false;

	//TIEMLINE
	//-----------------------------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CameraTransition;
 
	//la curva que queremos (en verdad no tengo ni idea)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* CrouchingCurve;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* MyTimeline;

	FOnTimelineFloat ProgressUpdate{};
 
	UFUNCTION()
	void CrouchUpdate(float val);
	//-----------------------------------------------------------------------------------------------------------------------------//


	//VAULTING
	//-----------------------------------------------------------------------------------------------------------------------------//
	FVector VaultStartPos;
	FVector VaultMiddlePos;
	FVector VaultLandingPos;

	bool bCanVault = false;
	bool bIsVaulting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMotionWarpingComponent * WarpingComp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Animations)
	class UAnimMontage* VaultMontage;

	FTimerHandle VaultingTimer;

	void Vaulting();
	void VaultingLength(FHitResult &OutHitResult);
	void VaultingMovmentWarping();
	void VaultingMovmentWarpingEnd();
	bool InRange();
	//-----------------------------------------------------------------------------------------------------------------------------//

	//HUD
	//-----------------------------------------------------------------------------------------------------------------------------//		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPlayerStats* Stats;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUD> HUDScreenClass;

	UPROPERTY()
	class UPlayerHUD* HUDScreen;
	
public:
	UPlayerHUD* GetPlayerHUD();
	//-----------------------------------------------------------------------------------------------------------------------------//

	UFUNCTION(BlueprintCallable)
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	//RUNING
	//-----------------------------------------------------------------------------------------------------------------------------//
	void SprintHandler();
	void SprintHandlerEnd();
	void Sprint();
	FTimerHandle TimerHandler;
	bool bSprinting = true;
	//-----------------------------------------------------------------------------------------------------------------------------//

	//ASSASSINATION
	//-----------------------------------------------------------------------------------------------------------------------------//
	void Assassination();
	UPROPERTY(EditAnywhere, Category = "Animations")
	class UAnimMontage* KnifeAnim;
	//-----------------------------------------------------------------------------------------------------------------------------//

	//ATTACK
	//-----------------------------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPlayerAttack* Attack;


	void SwordTrigger();

	FVector GetSwordTopPoint();
	FVector GetSwordBottomPoint();
	FVector GetKickPoint();
	//-----------------------------------------------------------------------------------------------------------------------------//

	//LOCK ENEMY
	//-----------------------------------------------------------------------------------------------------------------------------//
	void LookEnemy();

	class AActor* ActorToLock;
	//-----------------------------------------------------------------------------------------------------------------------------//

	//DODGE
	//-----------------------------------------------------------------------------------------------------------------------------//	
	void Dodge();
	void Dodging();

	UPROPERTY(EditAnywhere, Category = "Animations")
	class UAnimMontage* DodgeAnim;

	bool bIsDoding = false;

	FTimerHandle DodgeHandle;
	//-----------------------------------------------------------------------------------------------------------------------------//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UEquipmentSystem* EquipmentSystem;


};

