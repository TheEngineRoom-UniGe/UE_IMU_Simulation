// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPoseableMeshComponent.h"

TArray<FQuat> UMyPoseableMeshComponent::GetAallBoneOrientations()
{
	TArray<FBoneNode> tree = SkeletalMesh->GetSkeleton()->GetBoneTree();

	return TArray<FQuat>();
}
