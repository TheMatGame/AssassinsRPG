// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_ASSASIN_API UPlayerAttack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void SwordTrigger();
	void AttackPatern();
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SwordAttack();
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SwordAttackCombo();
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void StopCombo();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSaveAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int AttackIndex = 0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Animations)
	class UAnimMontage* Attack1Montage;
	void Attack1();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Animations)
	class UAnimMontage* Attack2Montage;
	void Attack2();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Animations)
	class UAnimMontage* Attack3Montage;
	void Attack3();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Animations)
	class UAnimMontage* Attack4Montage;
	void Attack4();

	FTimerHandle AttackTimer;
	FTimerHandle SwordTraceTimer;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void StartSwordTrace();
	void SwordTraceLoop();
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void StopSwordTrace();

	class ARPG_AssasinCharacter* Owner;

	//fot stab and kick
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SphereTrace();

	FTimerHandle DoOnceTimer;
	void ChangeDoOnce();

	bool DoOnce = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> SwordHitCameraShake;

	void PlaySwordHitSounds();

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* SwordHitSound;
	
	UPROPERTY(EditAnywhere, Category = "FXs")
	class UParticleSystem* BloodParticle;
};
