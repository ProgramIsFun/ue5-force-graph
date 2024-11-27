// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeGraph.h"

#include <random>

// #define ENABLE_LOGGING 1
#include "utillllllssss.h"
#include "Kismet/GameplayStatics.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)

AKnowledgeGraph::~AKnowledgeGraph()
{
	ll("AKnowledgeGraph::~AKnowledgeGraph", true, 2);
}


AKnowledgeGraph::AKnowledgeGraph()
	: Super()
{

	FNBodySimModule::Get().EndRendering();

	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
					TEXT("InstancedStaticMeshComponent"));
		
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

}

void AKnowledgeGraph::BeginDestroy()
{
	ll("AKnowledgeGraph::BeginDestroy", true, 2);
	
	FNBodySimModule::Get().EndRendering();
	

	Super::BeginDestroy();
}

void AKnowledgeGraph::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ll("AKnowledgeGraph::EndPlay", true, 2);
	ll("EndPlayReason: " + FString::FromInt((int)EndPlayReason), true, 2);
	// if (use_shaders)
	// {
	// 	FNBodySimModule::Get().EndRendering();
	// }
	Super::EndPlay(EndPlayReason);
}

void AKnowledgeGraph::BeginPlay()
{
	Super::BeginPlay();

	ClearLogFile();
	
	prepare();	
}


void AKnowledgeGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ll("sizeof(int) is: " + FString::FromInt(sizeof(int)), true, 2);
	// qq();
	// return;

	
	if (!prechecksucceeded)
	{
		ll("prechecksucceeded is false", true, 2);
		FNBodySimModule::Get().EndRendering();
		qq();
		return;
	}


	bool log = use_logging;


	iterations += 1;

	ll("TICK----------------------------------------------------------------------------"
	   "----------------------------------------------------------------------------", log);

	ll("iterations: " + FString::FromInt(iterations), log);


	if (iterations > maxiterations)
	{
		ll("iterations is greater than maxiterations", log);
		FNBodySimModule::Get().EndRendering();
		return;
	}


	ll("alpha: " + FString::SanitizeFloat(alpha), log);

	if (alpha < alphaMin)
	{
		ll("alpha is less than alphaMin", log);
		FNBodySimModule::Get().EndRendering();
		return;
	}

	alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
	ll("alpha: " + FString::SanitizeFloat(alpha), log);

	

	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");
	if (use_shaders)
	{
		if (use_constant_delta_time < 0)
		{
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, 1);
			
		}
		else
		{
			float DeltaTime = use_constant_delta_time;
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, 1);
		}
		
	}

	if (!use_shaders)
	{


		ll("apply forces", log);
		ApplyForces();

		ll("update actor location based on velocity", log);
		update_position_array_according_to_velocity_array();


		ll("Logging out The position of the first node: " + nodePositions[0].ToString(), log);
		
	}
	update_Node_world_position_according_to_position_array();

	
	
	if (1)
	{
		ll("update link position", log);
		update_link_position();
	}

}
