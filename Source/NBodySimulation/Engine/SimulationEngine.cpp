﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SimulationEngine.h"

#include "SimulationLogChannels.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NBodySimModule.h"

// Sets default values
ASimulationEngine::ASimulationEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
}

// Called when the game starts or when spawned
void ASimulationEngine::BeginPlay()
{
	Super::BeginPlay();
	
	if (!SimulationConfig)
	{
		UE_LOG(LogNBodySimulation, Error, TEXT("Failed to start simulation : SimulationConfig data asset has not been assigned in simulation engine."));

		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
		return;
	}

	// Compute static variables.
	SimParameters.ViewportWidth = SimulationConfig->CameraOrthoWidth;
	SimParameters.CameraAspectRatio = SimulationConfig->CameraAspectRatio;
	
	InitBodies();
	
	FNBodySimModule::Get().BeginRendering();
	FNBodySimModule::Get().InitWithParameters(SimParameters);
}

void ASimulationEngine::BeginDestroy()
{
	FNBodySimModule::Get().EndRendering();
	Super::BeginDestroy();
}

// Called every frame
void ASimulationEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SimParameters.DeltaTime = DeltaTime;
	FNBodySimModule::Get().UpdateDeltaTime(DeltaTime);
	
	UpdateBodiesPosition(DeltaTime);
}


void ASimulationEngine::InitBodies()
{
	check(InstancedStaticMeshComponent);
	check(SimulationConfig);

	
	SimParameters.Bodies.SetNumUninitialized(SimulationConfig->NumberOfBody + SimulationConfig->CustomBodies.Num());
	BodyTransforms.SetNumUninitialized(SimulationConfig->NumberOfBody + SimulationConfig->CustomBodies.Num());
	
	// Initialize the random Bodies with a default random position, velocity and mass depending on config.
	for (int32 Index = 0; Index < SimulationConfig->NumberOfBody; ++Index)
	{
		float RandomMass = FMath::FRandRange(SimulationConfig->InitialBodyMassRange.X, SimulationConfig->InitialBodyMassRange.Y);

		FVector2f RandomPosition(FMath::RandPointInCircle(SimulationConfig->BodySpawnCircleRadius));

		/**
		 *	For the velocity, we need to have a starting velocity for each body so that they are rotating into the circle clockwise.
		 *	This allow to have a nice starting movement.
		 *	With 0 starting velocity, we kind of have a Supernova.
		 *	RadialSpeedRate, once applied, allow to give bodies less velocity when spawning near center (0,0) and more and more near the edge of the spawning circle.
		 */
		float RadialSpeedRate = SimulationConfig->BodySpawnCircleRadius / RandomPosition.Size();
		FVector2f RandomVelocity
		{
			FMath::FRandRange(SimulationConfig->BodySpawnVelocityRange.X, SimulationConfig->BodySpawnVelocityRange.Y) / RadialSpeedRate,
			0
		};
		/** Trigonometry to rotate velocity in a clockwise movement in the circle. */
		RandomVelocity = RandomVelocity.GetRotated(90.0f + FMath::RadiansToDegrees(FMath::Atan2(RandomPosition.Y, RandomPosition.X)));
		
		float MeshScale = FMath::Sqrt(RandomMass) * SimulationConfig->MeshScaling;
		
		FTransform MeshTransform(
			FRotator(),
			FVector(FVector2D(RandomPosition), 0.0f),
			FVector(MeshScale, MeshScale, 1.0f)
		);
		
		BodyTransforms[Index] = MeshTransform;
	}

	// Initialize the additional bodies set in the config file.
	for (int32 Index = 0; Index < SimulationConfig->CustomBodies.Num(); ++Index)
	{



		FBodyConfigEntry CustomBodyEntry = SimulationConfig->CustomBodies[Index];
		
		float MeshScale = FMath::Sqrt(CustomBodyEntry.Mass) * SimulationConfig->MeshScaling;
		
		FTransform MeshTransform (
		FRotator(),
		FVector(FVector2D(CustomBodyEntry.SpawnPosition), 0.0f),
		FVector(MeshScale, MeshScale, 1.0f)
		);

		int32 BodyIndex = SimulationConfig->NumberOfBody + Index;
		BodyTransforms[BodyIndex] = MeshTransform;
	}

	/** Finally add instances to component to spawn them. */
	InstancedStaticMeshComponent->AddInstances(BodyTransforms, false);


	SimParameters.NumBodies = SimParameters.Bodies.Num();
	SimParameters.GravityConstant = SimulationConfig->GravitationalConstant;
}

void ASimulationEngine::UpdateBodiesPosition(float DeltaTime)
{}


