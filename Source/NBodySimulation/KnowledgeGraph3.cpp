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



bool AKnowledgeGraph::Maint(float DeltaTime)
{
	if (!prechecksucceeded)
	{
		ll("prechecksucceeded is false", true, 2);
		FNBodySimModule::Get().EndRendering();
		qq();
		return true;
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
		update_link_position();

		return true;
	}


	ll("alpha Before update: " + FString::SanitizeFloat(alpha), log);

	if (alpha < alphaMin)
	{
		ll("alpha is less than alphaMin", log);
		FNBodySimModule::Get().EndRendering();
		update_link_position();
		return true;
	}
	
	alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
	ll("alpha After update, pass to the gpu later: " + FString::SanitizeFloat(alpha), log);

	

	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");
	if (use_shaders)
	{
		if (use_constant_delta_time < 0)
		{
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, alpha);
		}
		else
		{
			float DeltaTime = use_constant_delta_time;
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, alpha );
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
	return false;
}


