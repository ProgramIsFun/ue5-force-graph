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

void AKnowledgeGraph::debug_test()
{
	if (1)
	{
		UStaticMeshComponent* CylinderMesh;

		if (debug111)
		{
			// Dynamically create the mesh component and attach it
			CylinderMesh = NewObject<UStaticMeshComponent>(this,
			                                               TEXT("CylinderMesh1")
			);
			CylinderMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CylinderMesh->RegisterComponent();  // Registers the component with the World so it gets rendered and updated

			CylinderMesh->SetWorldScale3D(FVector(10, 1, 1));
		
		
			CylinderMesh->SetStaticMesh(
				link_use_static_meshlinkMesh);
		
			// Set the position. 
			CylinderMesh->SetRelativeLocation(FVector(10, 10, 100));

		}


		if (debug112)
		{
			// Dynamically create the mesh component and attach it
			CylinderMesh = NewObject<UStaticMeshComponent>(this,
			                                               TEXT("CylinderMesh2")
			);
			CylinderMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CylinderMesh->RegisterComponent();  // Registers the component with the World so it gets rendered and updated

			CylinderMesh->SetWorldScale3D(FVector(1, 1, 10));
			if (0)
			{
				// Load the mesh
				static ConstructorHelpers::FObjectFinder<UStaticMesh>
					CylinderMeshAsset(TEXT("StaticMesh'/Game/Meshes/CylinderMesh.CylinderMesh'"));
				if (CylinderMeshAsset.Succeeded())
				{
					CylinderMesh->SetStaticMesh(CylinderMeshAsset.Object);
				}
			}
			else
			{
				CylinderMesh->SetStaticMesh(
					link_use_static_meshlinkMesh2
				);
			}
			// Set the position. 
			CylinderMesh->SetRelativeLocation(FVector(10, 10, 100));
		
		}
	}
	else
	{
		TArray<UStaticMeshComponent*> CylinderMeshes;

		// Loop to create 10 Cylinder Meshes
		for (int32 i = 0; i < 10; ++i)
		{
			UStaticMeshComponent* NewCylinderMesh = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("CylinderMesh%d"), i)));
			NewCylinderMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			NewCylinderMesh->RegisterComponent();

			// Set scale
			NewCylinderMesh->SetWorldScale3D(FVector(10, 1, 1));
    
			// Set static mesh
			NewCylinderMesh->SetStaticMesh(link_use_static_meshlinkMesh);
    
			// Set different relative locations for each cylinder to avoid overlap
			NewCylinderMesh->SetRelativeLocation(FVector(10, 10, 100 + i * 120));

			// Add to the array
			CylinderMeshes.Add(NewCylinderMesh);
		}
	}
}

void AKnowledgeGraph::BeginPlay()
{
	Super::BeginPlay();

	ClearLogFile();

	
	// debug_test();
		
	prepare(); 
	
}


void AKnowledgeGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(iterationsf<10)
	{
		ll("The reason of this section is because the first few frames seems "
	 "to be of sink between the gpu and the cpu. ", false, 2); 
		iterationsf+=1;
		return;
	}
	
	Maint(DeltaTime);

}
