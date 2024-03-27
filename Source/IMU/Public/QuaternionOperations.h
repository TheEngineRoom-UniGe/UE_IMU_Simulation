// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuaternionOperations.generated.h"

class USkeletalMesh;
/**
 * 
 */
UCLASS()


class IMU_API UQuaternionOperations : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintCallable, Category="QuaternionOperations")
	static FQuat RotatorToQuaternion(FRotator rotation);

	UFUNCTION(BlueprintCallable, Category = "QuaternionOperations")
	static FRotator QuaternionToRotator(FQuat rotation);

	UFUNCTION(BlueprintCallable, Category = "QuaternionOperations")
	static FString QuaternionToString(FQuat rotation);

	static std::string QuaternionToStdString(FQuat rotation);

	UFUNCTION(BlueprintCallable, Category = "QuaternionOperations")
	static FQuat StringToQuaternion(FString quatString);

	UFUNCTION(BlueprintCallable, Category = "QuaternionOperations")
	static void AddSocket(USkeletalMesh* skeleton);

};
