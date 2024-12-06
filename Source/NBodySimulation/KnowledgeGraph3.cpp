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
		SimParameters.shaderdebug = static_cast<unsigned int>(shaderdebug);
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);
	}
}


void AKnowledgeGraph::Updatmeterinshader(float DeltaTime)
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

bool AKnowledgeGraph::Earlyexit(bool log)
{
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
	return false;
}

void AKnowledgeGraph::CPUcalculate(bool log)
{
	if (iterations == 1)
	{
		ll("In the usual value. ", true, 2);
		ll("first node: " + nodePositions[0].ToString(), log);
		ll("second node: " + nodePositions[1].ToString(), log);
		ll("third node: " + nodePositions[2].ToString(), log);
	}


	// ll("apply forces", log);
	ApplyForces();

	// ll("update actor location based on velocity", log);
	update_position_array_according_to_velocity_array();


	ll("first node: " + nodePositions[0].ToString(), log);
	ll("second node: " + nodePositions[1].ToString(), log);
	ll("third node: " + nodePositions[2].ToString(), log);
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


	if (Earlyexit(log))
	{
		return true;
	}


	alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
	ll("alpha After update, pass to the gpu later: " + FString::SanitizeFloat(alpha), log);


	if (use_shaders)
	{
		gpugetpositions();
		
		Updatmeterinshader(DeltaTime);
	}else
	{
		CPUcalculate(log);
	}
	update_Node_world_position_according_to_position_array();


	if (true)
	{
		ll("update link position", log);
		update_link_position();
	}
	return false;
}

void AKnowledgeGraph::gpugetpositions()
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

	ll("First element position is: " + GPUOutputPositions[0].ToString(), use_logging, 2);
	ll("second element position is: " + GPUOutputPositions[1].ToString(), use_logging, 2);
	ll("third element position is: " + GPUOutputPositions[2].ToString(), use_logging, 2);
	
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
