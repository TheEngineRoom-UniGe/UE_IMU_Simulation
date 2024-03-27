// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtils.generated.h"

class ULocalPoseableMeshComponent;
class UQueueFileSaver;
/**
 * 
 */
UCLASS()
class IMU_API UStringUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "StringUtils")
	static void CreateAndEnqueueAnimHeader(TArray<FName> BoneList, UQueueFileSaver* qFileSaver);

	UFUNCTION(BlueprintCallable, Category = "StringUtils")
	static void CreateAndEnqueueIMUHeader(TArray<AActor*> IMUList, UQueueFileSaver* qFileSaver);

	UFUNCTION(BlueprintCallable, Category = "StringUtils")
	static void CreateAndEnqueueAnimLine(TArray<FName> BoneList, ULocalPoseableMeshComponent* PosMesh, float TStamp, UQueueFileSaver* qFileSaver);

	UFUNCTION(BlueprintCallable, Category = "StringUtils")
	static void CreateAndEnqueueIMULine(TArray<AActor*> IMUList, float TStamp, UQueueFileSaver* qFileSaver);

	UFUNCTION(BlueprintCallable, Category = "StringUtils")
	static FString LocationToString(FVector loc);

	UFUNCTION(BlueprintPure, Category = "StringUtils")
	static const FString GetAnimName(FAssetData asset);

	static std::string LocationToStdString(FVector loc);

};
