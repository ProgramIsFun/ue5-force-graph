#include "KnowledgeGraph.h"


void AKnowledgeGraph::prepare()
{
	if (use_instance_static_mesh_fornode)
	{
		if (!InstancedStaticMeshComponent)
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
		jnodessss
	);
	BodyTransforms.SetNumUninitialized(
		jnodessss);


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
	SimParameters.NumBodies = jnodessss;
	FNBodySimModule::Get().BeginRendering();
	FNBodySimModule::Get().InitWithParameters(SimParameters);
}

void AKnowledgeGraph::update_Node(float DeltaTime)
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


	update_Node_world_position_according_to_position_array();
}

void AKnowledgeGraph::Maintick(float DeltaTime)
{
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
		if (usedebuglinetrace)
		{
			update_link_position();
		}
		return;
	}


	ll("alpha: " + FString::SanitizeFloat(alpha), log);

	if (alpha < alphaMin)
	{
		ll("alpha is less than alphaMin", log);
		FNBodySimModule::Get().EndRendering();
		if (usedebuglinetrace)
		{
			update_link_position();
		}
		return;
	}

	alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
	ll("alpha: " + FString::SanitizeFloat(alpha), log);


	update_Node(DeltaTime);


	ll("update link position", log);
	update_link_position();
}

void AKnowledgeGraph::UpdateBodiesPosition()
{
}
