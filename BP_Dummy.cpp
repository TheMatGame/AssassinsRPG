// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_Dummy.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "RPG_AssasinCharacter.h"
#include "PlayerStats.h"



// Sets default values
ABP_Dummy::ABP_Dummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AssassinRef = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AssassinREF"));
	AssassinRef->SetupAttachment(RootComponent);

	AssassinationText = CreateDefaultSubobject<UWidgetComponent>(TEXT("AssassinationText"));
	AssassinationText->SetupAttachment(RootComponent);
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);

	AssassinationRange = CreateDefaultSubobject<USphereComponent>(TEXT("AssassinationRange"));
	AssassinationRange->SetupAttachment(GetMesh());

	Stats = CreateDefaultSubobject<UPlayerStats>(TEXT("Stats"));

	

}

// Called when the game starts or when spawned
void ABP_Dummy::BeginPlay()
{
	Super::BeginPlay();
	AssassinationRange->SetGenerateOverlapEvents(true);
	AssassinationRange->OnComponentBeginOverlap.AddDynamic(this, &ABP_Dummy::OnComponentBeginOverlap);
	AssassinationRange->OnComponentEndOverlap.AddDynamic(this, &ABP_Dummy::OnComponentEndOverlap);

	if (Stats)	Stats->CallHUD();
}

// Called every frame
void ABP_Dummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector TextLocation = AssassinationText->GetComponentLocation();

	AssassinationText->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(TextLocation, CameraLocation));
	HealthBar->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(TextLocation, CameraLocation));

}

// Called to bind functionality to input
void ABP_Dummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABP_Dummy::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString aux = (TEXT("Overlap start"));
	UKismetSystemLibrary::PrintString(GetWorld(), aux, false, true);

	//todo hace overlap del personaje y solo quiero que lo haga la mesh en el if poner que el otro comp es mesh

	ARPG_AssasinCharacter* OverlappedActor = Cast<ARPG_AssasinCharacter>(OtherActor);
	if (OverlappedActor)
	{
		AssassinationText->SetVisibility(true);
	}
	
}

void ABP_Dummy::OnComponentEndOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString aux = (TEXT("Overlap end"));
	UKismetSystemLibrary::PrintString(GetWorld(), aux, false, true);

	ARPG_AssasinCharacter* OverlappedActor = Cast<ARPG_AssasinCharacter>(OtherActor);
	if (OverlappedActor)
	{
		AssassinationText->SetVisibility(false);
	}
	
}

void ABP_Dummy::StealthBackAssassin(FVector &RefLoc, FRotator &RefRot)
{
	PlayAnimMontage(KnifeAnim);
	GetWorldTimerManager().SetTimer(RagdollTimer, this, &ABP_Dummy::Ragdoll, 2.0f, false);
	AssassinationText->SetVisibility(false);
	RefLoc = AssassinRef->GetComponentLocation();
	RefRot = AssassinRef->GetComponentRotation();
}

void ABP_Dummy::Ragdoll()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

float ABP_Dummy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	
	
	int RandomNumber = UKismetMathLibrary::RandomIntegerInRange(0, HitAnims.Num()-1);
	PlayAnimMontage(HitAnims[RandomNumber]);

	FVector LaunchVelocity = (DamageCauser->GetActorForwardVector()) * 700.f;
	LaunchCharacter(LaunchVelocity, false, false);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntSound, GetActorLocation());

	bool bIsAlive = Stats->DecreaseHealth(DamageAmount);

	if (!bIsAlive)
	{
		GetMesh()->SetSimulatePhysics(true);
		Ragdoll();
		HealthBar->SetVisibility(false);
	}

	return DamageApplied;
}

UDummyHealthBar* ABP_Dummy::GetHealthBar()
{
	return Cast<UDummyHealthBar, UUserWidget>(HealthBar->GetWidget());
}


