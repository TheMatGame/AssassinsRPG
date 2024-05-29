// Fill out your copyright notice in the Description page of Project Settings.


#include "AssassinationRange.h"
#include "Components/PrimitiveComponent.h"

void UAssassinationRange::BeginComponentOverlap(const FOverlapInfo& OtherOverlap, bool bDoNotifies)
{
    Super::BeginComponentOverlap(OtherOverlap, bDoNotifies);
    UE_LOG(LogTemp, Warning, TEXT("OVERLAPING"));
}