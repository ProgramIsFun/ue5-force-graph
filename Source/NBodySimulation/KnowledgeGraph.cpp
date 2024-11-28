// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeGraph.h"
#include "utillllllssss.h"

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

	
	Maintick(DeltaTime);
}
