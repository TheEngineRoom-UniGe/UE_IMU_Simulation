// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IMUSensor.generated.h"

UCLASS()
class AIMUSensor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIMUSensor();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IMU")
	float Frequency;
	UPROPERTY(BlueprintReadWrite, Category = "IMU")
	float DeltaT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accelerometer")
	float ACC_fullrange;
	UPROPERTY(BlueprintReadOnly, Category = "Accelerometer")
	FVector ACC_val;	
	UPROPERTY(BlueprintReadOnly, Category = "Accelerometer")
	FVector SPD_val;
	UPROPERTY(BlueprintReadOnly, Category = "Accelerometer")
	FVector DeltaS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gyroscope")
	float GYR_fullrange;
	UPROPERTY(BlueprintReadOnly, Category = "Gyroscope")
	FVector GYR_val;

	// Values for MagneticDip can be found at https://www.magnetic-declination.com/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Magnetometer")
	float MagneticDip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	float MagneticDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	float MagneticFieldStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magnetometer")
	float MAG_fullrange;
	UPROPERTY(BlueprintReadOnly, Category = "Magnetometer")
	FVector MAG_val;
	UPROPERTY(BlueprintReadWrite, Category = "Magnetometer")
	AActor* ANorthPole;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FVector GetAccelerometerReadings();

	UFUNCTION(BlueprintCallable)
	FVector GetGyroscopeReadings();

	UFUNCTION(BlueprintCallable)
	inline FVector GetMagnetometerReadings(FQuat orientation);

	inline FVector DeriveQuaternion(FQuat curr, FQuat prev, double deltaT);

	inline FVector DeriveVector(FVector curr, FVector prev, double deltaT);

	inline FQuat Right2LeftQuaternion(FQuat Quat);

	UFUNCTION(BlueprintCallable)
	void IMUCycle();

	UFUNCTION(BlueprintCallable, Category = "Save")
	static bool FileSaveString(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintPure, Category = "Save")
	static bool FileLoadString(FString FileNameA, FString& SaveTextA);

private:

	FVector _previousLocation;
	FVector _previousVelocity;

	FQuat _previousRotation;

	double _previousTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
