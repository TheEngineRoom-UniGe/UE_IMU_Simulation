// Fill out your copyright notice in the Description page of Project Settings.


#include "IMUSensor.h"
#include "Engine.h" 

// Sets default values
AIMUSensor::AIMUSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Frequency = 30.0f;

	// Default to Genova values -> 60° 37' -> 1.05796041 rad
	MagneticDip = 1.05796041f;
	// Default to Genova values -> 47403.0 nT
	MagneticFieldStrength = 47403.0;
	MagneticDistance = 100000.0f;

	_previousLocation = GetActorLocation();
	_previousVelocity = FVector::ZeroVector;
	_previousRotation = GetActorRotation().Quaternion();
	_previousTime = FPlatformTime::Seconds();

}

// Called when the game starts or when spawned
void AIMUSensor::BeginPlay()
{
	Super::BeginPlay();

	FVector MagneticLocation = FVector(MagneticDistance, 0.0f, -cos(MagneticDip) * MagneticDistance);

	FActorSpawnParameters params;
	params.Name = FName("NorthPole");

	FVector scale = FVector(1.0, 1.0, 1.0);

	FTransform *tf = new FTransform(FQuat::Identity.Rotator(), MagneticLocation, scale);

	//ANorthPole = GetWorld()->SpawnActor(AActor::StaticClass(), tf, params);
	
}

FVector AIMUSensor::GetAccelerometerReadings()
{
	return FVector();
}

FVector AIMUSensor::GetGyroscopeReadings()
{
	return FVector();
}

FVector AIMUSensor::GetMagnetometerReadings(FQuat orientation)
{
	if (ensure(ANorthPole))
	{
		FVector MagneticDirection = ANorthPole->GetActorLocation() - GetActorLocation();
		MagneticDirection.Normalize();
		MagneticDirection *= MagneticFieldStrength;

		FQuat inv = orientation.Inverse();
	
		return inv.RotateVector(MagneticDirection);
	}
	return FVector::ZeroVector;
}

FVector AIMUSensor::DeriveQuaternion(FQuat curr, FQuat prev, double deltaT)
{
	FVector AngularVelocities;

	AngularVelocities.X = (2 / deltaT) * (prev.W * curr.X - prev.X * curr.W - prev.Y * curr.Z + prev.Z * curr.Y);
	AngularVelocities.Y = (2 / deltaT) * (prev.W * curr.Y + prev.X * curr.Z - prev.Y * curr.W - prev.Z * curr.X);
	AngularVelocities.Z = (2 / deltaT) * (prev.W * curr.Z - prev.X * curr.Y + prev.Y * curr.X - prev.Z * curr.W);

	return AngularVelocities;
}

FVector AIMUSensor::DeriveVector(FVector curr, FVector prev, double deltaT)
{
	return ( curr - prev ) / deltaT;
}

// Called every frame
void AIMUSensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

inline FQuat AIMUSensor::Right2LeftQuaternion(FQuat Quat)
{
	return FQuat(-Quat.X, -Quat.Z, -Quat.Y, Quat.W);
}

void AIMUSensor::IMUCycle()
{
	// Get updated position and orientation
	FVector tempLoc = GetActorLocation() / 100.0f;
	FQuat tempRot = GetActorRotation().Quaternion();

	// Derive position and orientation
	FVector newVelocity = DeriveVector(tempLoc, _previousLocation, DeltaT);
	SPD_val = newVelocity;
	DeltaS = newVelocity - _previousVelocity;
	ACC_val = DeriveVector(newVelocity, _previousVelocity, DeltaT);

	//GYR_val = -DeriveQuaternion(tempRot, _previousRotation, deltaT);

	FVector MAG_val_t = GetMagnetometerReadings(tempRot);
	if (!MAG_val_t.IsZero()) {
		MAG_val = MAG_val_t;
	}

	// Update position, orientation and deltaT
	_previousVelocity = newVelocity;
	_previousLocation = tempLoc;
	_previousRotation = tempRot;
}

bool AIMUSensor::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::ProjectDir() + FileNameB), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

bool AIMUSensor::FileLoadString(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ProjectDir() + FileNameA));
}