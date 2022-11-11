// Fill out your copyright notice in the Description page of Project Settings.


#include "HoudiniUnrealActor.h"

// Sets default values
AHoudiniUnrealActor::AHoudiniUnrealActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AHoudiniUnrealActor::ForceTriggerUpdata()
{
	RerunConstructionScripts();
}

// Called when the game starts or when spawned
void AHoudiniUnrealActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoudiniUnrealActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

