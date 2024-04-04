// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/Tuple.h"
#include "IMUSensor.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class AIMUSensor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIMUSensor();

	UFUNCTION(BlueprintCallable, Category = "IMU")
	FVector GetMagneticField();
	FVector GetMagneticField(FQuat currentRot);

	UFUNCTION(BlueprintCallable, Category = "IMU")
	FVector GetAngularVelocity();
	FVector GetAngularVelocity(FQuat currentRot, float dT);

	UFUNCTION(BlueprintCallable, Category = "IMU")
	FVector GetAcceleration(float dT);
	FVector GetAcceleration(FQuat currentRot, float dT);

	UFUNCTION(BlueprintCallable, Category = "IMU")
	float GetDT(float currentTime);

	FRotator GetCapsuleAbsoluteRotation();


protected:

	UPROPERTY(BlueprintReadWrite, Category = "IMU")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IMU")
	UStaticMeshComponent* IMUmesh;

	// Values for MagneticDip can be found at https://www.magnetic-declination.com/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Magnetometer")
	float MagneticDip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	float MagneticDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	float MagneticFieldStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	float MAG_fullrange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	FVector NorthPoleLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Magnetometer")
	FVector MAG_val;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FVector _previousVel;
	FQuat _previousRot;

	float _lastIMUTstamp;
};
