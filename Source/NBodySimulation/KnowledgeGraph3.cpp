#include "KnowledgeGraph.h"


void AKnowledgeGraph::prepare()
{
	
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

	

	timeThisMemberFunction(
		"AKnowledgeGraph::initializeNodePosition",
		&AKnowledgeGraph::initializeNodePosition);


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
		SimParameters.shaderdebug = static_cast<unsigned int>(use_shaders_debug);
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);
	}
}


void AKnowledgeGraph::update_parameter_in_shader(float DeltaTime)
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
		FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, alpha);
	}
}

bool AKnowledgeGraph::early_exit(bool log)
{
	if (iterations > max_iterations)
	{
		ll("iterations is greater than max_iterations", log);
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
	return false;
}

void AKnowledgeGraph::cpu_calculate()
{
	bool log = use_logging;

	// ll("apply forces", log);
	apply_force();

	// ll("update actor location based on velocity", log);
	update_position_array_according_to_velocity_array();

}

void AKnowledgeGraph::update_position_array(bool log)
{
	if (use_shaders)
	{
		gpu_get_positions();
		
	}else
	{
		cpu_calculate();
	}
}

void AKnowledgeGraph::update_alpha()
{
	bool log=true;
	alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
	ll("alpha After update, pass to the gpu later: " + FString::SanitizeFloat(alpha), log);
}

void AKnowledgeGraph::print_out_location_of_the_node()
{
	bool log=true;
	ll("Before update. ", log);
	ll("first element. " + nodePositions[0].ToString(), log);
	ll("second element. " + nodePositions[1].ToString(), log);
	ll("third element. " + nodePositions[2].ToString(), log);
}

bool AKnowledgeGraph::Maint(float DeltaTime)
{
	bool log = use_logging;
	
	iterations += 1;
	
	ll("TICK----------------------------------------------------------------------------"
	   "----------------------------------------------------------------------------", log);

	ll("iterations: " + FString::FromInt(iterations), log);


	if (early_exit(log))
	{
		return true;
	}

	update_alpha();
	
	print_out_location_of_the_node();

	update_position_array(log);

	print_out_location_of_the_node();
	
	update_node_world_position_according_to_position_array();
	
	if (update_link_before_stabilize)
	{
		ll("update link position", log);
		update_link_position();
	}
	
	if (use_shaders){
	
		update_parameter_in_shader(DeltaTime);
	}
	return false;
}

void AKnowledgeGraph::gpu_get_positions()
{
	// Retrieve GPU computed bodies position.
	TArray<FVector3f> GPUOutputPositions = FNBodySimModule::Get().GetComputedPositions();
	if (GPUOutputPositions.Num() != SimParameters.Bodies.Num())
	{
		ll("Size differ. Bodies (" +
		   FString::FromInt(SimParameters.Bodies.Num()) + ") Output(" + FString::FromInt(GPUOutputPositions.Num()) +
		   ")", true, 2);

		GPUvalid = false;
		return;
	}else
	{
		ll("Size is same. Bodies (" +
		   FString::FromInt(SimParameters.Bodies.Num()) + ") Output(" + FString::FromInt(GPUOutputPositions.Num()) +
		   ")", use_logging, 2);
	}

	TArray<float> alphas = FNBodySimModule::Get().GetComputedAlphas();
	ll("alpha: " + FString::SanitizeFloat(alphas[0]), use_logging, 2);
	ll("alpha1: " + FString::SanitizeFloat(alphas[1]), use_logging, 2);

	// ll("First element position is: " + GPUOutputPositions[0].ToString(), use_logging, 2);
	// ll("second element position is: " + GPUOutputPositions[1].ToString(), use_logging, 2);
	// ll("third element position is: " + GPUOutputPositions[2].ToString(), use_logging, 2);
	//
	if (iterations == 1)
	{
		ll("First iteration gpu is useless!!!!!!!!!!!!!!!!!!!!!!!!! ", use_logging, 2);
		GPUvalid = false;
		return;
	}

	for (int i = 0; i < SimParameters.Bodies.Num(); i++)
	{
		FVector NewPosition = FVector(GPUOutputPositions[i]);
		nodePositions[i] = NewPosition;
	}
	GPUvalid = true;

	
}
