// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttack.h"
#include "RPG_AssasinCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlayerAttack::UPlayerAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAttack::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner<ARPG_AssasinCharacter>();
	
}


// Called every frame
void UPlayerAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerAttack::SwordTrigger()
{
	if (bCanAttack)
	{
		SwordAttack();
	}
}

void UPlayerAttack::AttackPatern()
{
	if (AttackIndex == 0)
	{
		Owner->GetWorldTimerManager().SetTimer(AttackTimer, this, &UPlayerAttack::Attack1, 0.1, false);
		++AttackIndex;
	}
	else if (AttackIndex == 1)
	{
		Owner->GetWorldTimerManager().SetTimer(AttackTimer, this, &UPlayerAttack::Attack2, 0.1, false);
		++AttackIndex;
	}
	else if (AttackIndex == 2)
	{
		Owner->GetWorldTimerManager().SetTimer(AttackTimer, this, &UPlayerAttack::Attack3, 0.1, false);
		++AttackIndex;
	}
	else if (AttackIndex == 3)
	{
		Owner->GetWorldTimerManager().SetTimer(AttackTimer, this, &UPlayerAttack::Attack4, 0.1, false);
		++AttackIndex;
		
	}
}

void UPlayerAttack::SwordAttack()
{
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		AttackPatern();
	}
}

void UPlayerAttack::SwordAttackCombo()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		AttackPatern();
	}
	else
	{
		StopCombo();
	}
}

void UPlayerAttack::StopCombo()
{
	bIsAttacking = false;
	AttackIndex = 0;
}

void UPlayerAttack::StartSwordTrace()
{
	Owner->GetWorldTimerManager().SetTimer(SwordTraceTimer, this, &UPlayerAttack::SwordTraceLoop, 0.01, true);
}

void UPlayerAttack::SwordTraceLoop()
{
	FVector StartLocation = Owner->GetSwordTopPoint();
	FVector EndLocation = Owner->GetSwordBottomPoint();

	TArray<FHitResult> SwordHit;
	TArray<AActor*> ActorsToIngore;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLocation, EndLocation, 12.f, FName(TEXT("Pawn")), false, ActorsToIngore, EDrawDebugTrace::None, SwordHit, true);

	for (FHitResult HitActor : SwordHit)
	{
		if (bHit && HitActor.GetActor()->ActorHasTag(FName(TEXT("Damageable"))))
		{
			if (DoOnce)
			{
				UGameplayStatics::ApplyDamage(HitActor.GetActor(), 10.f, Owner->GetController(), GetOwner(), UDamageType::StaticClass());
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), SwordHitCameraShake, HitActor.Location, 0.f, 200.f);
				Owner->GetWorldTimerManager().SetTimer(DoOnceTimer, this, &UPlayerAttack::ChangeDoOnce, 0.5f, false);
				DoOnce = false;
				PlaySwordHitSounds();

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
					BloodParticle, 
					HitActor.Location, 
					UKismetMathLibrary::MakeRotFromXY(HitActor.Normal, HitActor.Normal),
					FVector(0.4),
					true,
					EPSCPoolMethod::AutoRelease
				);
			}
		}

	}
	

}

void UPlayerAttack::ChangeDoOnce()
{
	DoOnce = true;
}

void UPlayerAttack::StopSwordTrace()
{
	Owner->GetWorldTimerManager().ClearTimer(SwordTraceTimer);
}

void UPlayerAttack::Attack1()
{
	if (!Attack1Montage) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attack Montage"));
		return;
	}
	Owner->PlayAnimMontage(Attack1Montage);
}

void UPlayerAttack::Attack2()
{
	if (!Attack2Montage) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attack Montage"));
		return;
	}
	Owner->PlayAnimMontage(Attack2Montage);
}

void UPlayerAttack::Attack3()
{
	if (!Attack3Montage) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attack Montage"));
		return;
	}
	Owner->PlayAnimMontage(Attack3Montage);
}

void UPlayerAttack::Attack4()
{
	if (!Attack4Montage) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attack Montage"));
		return;
	}
	Owner->PlayAnimMontage(Attack4Montage);
}

void UPlayerAttack::SphereTrace()
{
	TArray<AActor*> ActorsToIngore;

	FVector aux = Owner->GetKickPoint();
	FHitResult Kick;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), aux, aux, 25.f, TraceTypeQuery1, false, ActorsToIngore, EDrawDebugTrace::None, Kick, true);

	if (bHit) {
		UGameplayStatics::ApplyDamage(Kick.GetActor(), 5.f, Owner->GetController(), GetOwner(), UDamageType::StaticClass());
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), SwordHitCameraShake, Kick.Location, 0.f, 200.f);
		PlaySwordHitSounds();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
			BloodParticle, 
			Kick.Location, 
			UKismetMathLibrary::MakeRotFromXY(Kick.Normal, Kick.Normal),
			FVector(0.4),
			true,
			EPSCPoolMethod::AutoRelease
		);
	}
}

void UPlayerAttack::PlaySwordHitSounds()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordHitSound, GetOwner()->GetActorLocation());
}



