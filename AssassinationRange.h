// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "AssassinationRange.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ASSASIN_API UAssassinationRange : public USphereComponent
{
	GENERATED_BODY()

public:

	void BeginComponentOverlap(const FOverlapInfo& OtherOverlap, bool bDoNotifies);
	
};
