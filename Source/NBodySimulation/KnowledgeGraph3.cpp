#include "KnowledgeGraph.h"


void AKnowledgeGraph::prepare()
{
	
	if (
		!use_shaders && !use_actor_fornode
	)
	{
		ll("If CPU we must use actor for node for right now. ", 1, 2);
		prechecksucceeded = false;
		qq();
		return;
	}
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

	if (use_shaders)
	{
		SimParameters.Bodies.SetNumUninitialized(
		jnodessss
		);
		BodyTransforms.SetNumUninitialized(
			jnodessss);
	}

	timeThisMemberFunction(
		"AKnowledgeGraph::initializeNodePosition",
		&AKnowledgeGraph::initializeNodePosition);

	if (use_instance_static_mesh_fornode)
	{
		InstancedStaticMeshComponent->AddInstances(BodyTransforms, false);
	}
	
	if (!use_shaders)
	{
		if (0)
		{
			update_Node_world_position_according_to_position_array();
		}
	}

	timeThisMemberFunction(
		"AKnowledgeGraph::CalculateBiasstrengthOflinks",
		&AKnowledgeGraph::CalculateBiasstrengthOflinks);


	if (use_shaders)
	{
		SimParameters.ViewportWidth = 8000.0;
		SimParameters.CameraAspectRatio = 1.777778;
		SimParameters.GravityConstant = 1000.0;
		SimParameters.NumBodies = jnodessss;
		SimParameters.alphaS = 1;
		SimParameters.shaderdebug=static_cast<unsigned int>(shaderdebug);
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);
	}

}


