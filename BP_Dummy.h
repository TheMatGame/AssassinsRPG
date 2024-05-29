// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StealthBackAssassin.h"
#include "DummyHealthBar.h"
#include "BP_Dummy.generated.h"

UCLASS()
class RPG_ASSASIN_API ABP_Dummy : public ACharacter, public IStealthBackAssassin
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABP_Dummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	class USkeletalMeshComponent* AssassinRef;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	class USphereComponent* AssassinationRange;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	class UWidgetComponent* AssassinationText;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	class UWidgetComponent* HealthBar;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	class UPlayerStats* Stats;

public:
	UFUNCTION(BlueprintNativeEvent)
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void StealthBackAssassin(FVector &RefLoc, FRotator &RefRot) override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Animations)
	class UAnimMontage* KnifeAnim;
	FTimerHandle RagdollTimer;

	void Ragdoll();

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* DeathSound;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimMontage*> HitAnims;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* GruntSound;
	
	UDummyHealthBar* GetHealthBar();
	
};
