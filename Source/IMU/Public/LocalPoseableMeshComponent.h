// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "LocalPoseableMeshComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class IMU_API ULocalPoseableMeshComponent : public UPoseableMeshComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
    void SetBoneLocalTransformByName(const FName& BoneName, const FTransform& InTransform);

    UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
    void SetBoneLocalRotationByName(const FName& BoneName, const FQuat& InRotation);

    UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
    void SetBoneLocalLocationByName(const FName& BoneName, const FVector& InLocation);

    UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
    FTransform GetBoneLocalTransformByName(const FName& BoneName);

    UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
    FQuat GetBoneLocalRotationByName(const FName& BoneName);

    UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
    FVector GetBoneLocalLocationByName(const FName& BoneName);

};
