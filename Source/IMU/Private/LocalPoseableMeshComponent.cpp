// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalPoseableMeshComponent.h"

void ULocalPoseableMeshComponent::SetBoneLocalTransformByName(const FName& BoneName, const FTransform& InTransform)
{
    if (!SkeletalMesh || !RequiredBones.IsValid())
    {
        return;
    }

    int32 boneIndex = GetBoneIndex(BoneName);
    if (boneIndex >= 0 && boneIndex < BoneSpaceTransforms.Num())
    {
        BoneSpaceTransforms[boneIndex] = InTransform;
        MarkRefreshTransformDirty();
    }
}

FTransform ULocalPoseableMeshComponent::GetBoneLocalTransformByName(const FName& BoneName)
{


    FTransform zeroTransform;
    if (!SkeletalMesh || !RequiredBones.IsValid())
    {
        return zeroTransform;
    }

    //SkinnedMes

    int32 boneIndex = GetBoneIndex(BoneName);
    if (boneIndex >= 0 && boneIndex < BoneSpaceTransforms.Num())
    {
        //UE_LOG(LogTemp, Warning, TEXT("BoneIndex, %s"), *FString::FromInt(boneIndex));
        return BoneSpaceTransforms[boneIndex];
    }
    //UE_LOG(LogTemp, Warning, TEXT("BoneIndex, %s"), *FString::FromInt(boneIndex));
    return zeroTransform;
}

FQuat ULocalPoseableMeshComponent::GetBoneLocalRotationByName(const FName& BoneName)
{
    FQuat rot = GetBoneLocalTransformByName(BoneName).GetRotation();

    return rot;
}

void ULocalPoseableMeshComponent::SetBoneLocalRotationByName(const FName& BoneName, const FQuat& InRotation)
{
    FTransform tf = GetBoneLocalTransformByName(BoneName);
    tf.SetRotation(InRotation);
    SetBoneLocalTransformByName(BoneName, tf);

}

FVector ULocalPoseableMeshComponent::GetBoneLocalLocationByName(const FName& BoneName)
{
    FVector loc = GetBoneLocalTransformByName(BoneName).GetLocation();

    return loc;
}

void ULocalPoseableMeshComponent::SetBoneLocalLocationByName(const FName& BoneName, const FVector& InLocation)
{
    FTransform tf = GetBoneLocalTransformByName(BoneName);
    tf.SetLocation(InLocation);
    SetBoneLocalTransformByName(BoneName, tf);

}