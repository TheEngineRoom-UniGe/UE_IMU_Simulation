// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "MyPoseableMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class IMU_API UMyPoseableMeshComponent : public UPoseableMeshComponent
{
	GENERATED_BODY()

protected:
	TArray<FQuat> BoneOrientations;

	TArray<FQuat> GetAallBoneOrientations();

	
};
