#include "KnowledgeGraph.h"


void AKnowledgeGraph::prepare()
{
	
	
	if (use_instance_static_mesh_fornode)
	{

		if(!InstancedStaticMeshComponent)
		{
			prechecksucceeded = false;
			qq();
			return;
		}
		
	}


	if (use_tick_interval)
	{
		UE_LOG(LogTemp, Warning, TEXT("Restricting tick interval"));

		PrimaryActorTick.TickInterval = tick_interval;
	}

	// generateGraph();
	timeThisMemberFunction(
		"AKnowledgeGraph::generateGraph",
		&AKnowledgeGraph::generateGraph
	);

	
		SimParameters.Bodies.SetNumUninitialized(
			jnodes1
		);
		BodyTransforms.SetNumUninitialized(
			jnodes1);
	

	timeThisMemberFunction(
		"AKnowledgeGraph::initializeNodePosition",
		&AKnowledgeGraph::initializeNodePosition);

	if (use_instance_static_mesh_fornode)
	{
		InstancedStaticMeshComponent->AddInstances(BodyTransforms, false);
	}
	
	

	timeThisMemberFunction(
		"AKnowledgeGraph::CalculateBiasstrengthOflinks",
		&AKnowledgeGraph::CalculateBiasstrengthOflinks);


	
		SimParameters.ViewportWidth = 8000.0;
		SimParameters.CameraAspectRatio = 1.777778;
		SimParameters.GravityConstant = 1000.0;
		SimParameters.NumBodies = jnodes1;
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);
		
	

}

void AKnowledgeGraph::UpdateBodiesPosition()
{
	
}
