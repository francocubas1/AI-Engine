// Fill out your copyright notice in the Description page of Project Settings.


#include "NodePooling.h"



// Sets default values
ANodePooling::ANodePooling()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ANodePooling::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePool();
}

void ANodePooling::InitializePool()
{
	for (int32 i = 0; i < PoolSize; i++)
	{
		AGridNodeActorBase* WallNode = GetWorld()->SpawnActor<AGridNodeActorBase>(WallNodeClass);
		AGridNodeActorBase* PathNode = GetWorld()->SpawnActor<AGridNodeActorBase>(PathNodeClass);
		WallPool.Enqueue(WallNode);
		PathPool.Enqueue(PathNode);
	}

	StartNode = GetWorld()->SpawnActor<AGridNodeActorBase>(StartNodeClass);
	GoalNode = GetWorld()->SpawnActor<AGridNodeActorBase>(GoalNodeClass);

}

void ANodePooling::DoublePoolSize()
{
	int32 CurrentSize = IncrementPoolSize;
	for (int32 i = 0; i < CurrentSize; i++)
	{
		AGridNodeActorBase* WallNode = GetWorld()->SpawnActor<AGridNodeActorBase>(WallNodeClass);
		AGridNodeActorBase* PathNode = GetWorld()->SpawnActor<AGridNodeActorBase>(PathNodeClass);
		WallPool.Enqueue(WallNode);
		PathPool.Enqueue(PathNode);
	}
}

void ANodePooling::GetWallNodeFromPool(AGridNodeActorBase*& OutNode)
{
	if (!WallPool.Dequeue(OutNode))
	{
		DoublePoolSize();
		WallPool.Dequeue(OutNode);
	}
	if (OutNode)
	{
		OutNode->SetActorHiddenInGame(false);
	}
}

void ANodePooling::GetPathNodeFromPool(AGridNodeActorBase*& OutNode)
{
	if (!PathPool.Dequeue(OutNode))
	{
		DoublePoolSize();
		PathPool.Dequeue(OutNode);
	}
	if (OutNode)
	{
		OutNode->SetActorHiddenInGame(false);
	}
}

void ANodePooling::GetNodeFromPool(EGridActorType NodeType, AGridNodeActorBase*& OutNode)
{
	switch (NodeType)
	{
	case EGridActorType::Start:
		OutNode = StartNode;
		break;
	case EGridActorType::Goal:
		OutNode = GoalNode;
		break;
	case EGridActorType::Wall:
		GetWallNodeFromPool(OutNode);
		break;
	case EGridActorType::Path:
		GetPathNodeFromPool(OutNode);
		break;
	default:
		OutNode = nullptr;
	}
}


