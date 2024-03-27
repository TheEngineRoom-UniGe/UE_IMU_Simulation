// Fill out your copyright notice in the Description page of Project Settings.


#include "IMUSensor.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h" 

// Sets default values
AIMUSensor::AIMUSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Default to Genova values -> 60° 37' -> 1.05796041 rad
	MagneticDip = 1.05796041f;
	// Default to Genova values -> 47403.0 nT
	MagneticFieldStrength = 47403.0;
	MagneticDistance = 100000.0f;

	NorthPoleLocation = FVector(MagneticDistance, 0.0f, -MagneticDistance / cos(MagneticDip));

	//IMUmesh = CreateDefaultSubobject<UStaticMeshComponent>("IMUmesh");
	//IMUmesh->SetupAttachment(RootComponent);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleVelComp");
	CapsuleComp->SetupAttachment(RootComponent);

	_previousVel = FVector::ZeroVector;
	_previousRot = FQuat::Identity;

	_lastIMUTstamp = 0.0f;

}

// Called when the game starts or when spawned
void AIMUSensor::BeginPlay()
{
	Super::BeginPlay();

}

//TTuple<FVector, FVector, FVector> AIMUSensor::GetIMUvalues(float dT) {
//	FQuat currentRot = CapsuleComp->GetComponentRotation().Quaternion();
//
//	FVector B = GetMagneticField(currentRot);
//	FVector Acc = GetAcceleration(currentRot, dT);
//	FVector W = GetAngularVelocity(currentRot, dT);
//
//	return TTuple<FVector, FVector, FVector>(B, Acc, W);
//}


FVector AIMUSensor::GetMagneticField(FQuat currentRot)
{

	FVector MagneticDirection = NorthPoleLocation - GetActorLocation();
	MagneticDirection.Normalize();
	MagneticDirection *= MagneticFieldStrength;

	FQuat inv = currentRot.Inverse();
	//FQuat currentRot = CapsuleComp->GetComponentRotation().Quaternion();
	return inv.RotateVector(MagneticDirection);
	
}


FVector AIMUSensor::GetAngularVelocity(float dT)
{
	FVector AngularVelocities;

	FQuat currentRot = CapsuleComp->GetComponentRotation().Quaternion();

	AngularVelocities.X = (2 / dT) * (_previousRot.W * currentRot.X - _previousRot.X * currentRot.W - _previousRot.Y * currentRot.Z + _previousRot.Z * currentRot.Y);
	AngularVelocities.Y = (2 / dT) * (_previousRot.W * currentRot.Y + _previousRot.X * currentRot.Z - _previousRot.Y * currentRot.W - _previousRot.Z * currentRot.X);
	AngularVelocities.Z = (2 / dT) * (_previousRot.W * currentRot.Z - _previousRot.X * currentRot.Y + _previousRot.Y * currentRot.X - _previousRot.Z * currentRot.W);

	_previousRot = currentRot;

	return AngularVelocities;
}

FVector AIMUSensor::GetAngularVelocity(FQuat currentRot, float dT)
{
	FVector AngularVelocities;

	AngularVelocities.X = (2 / dT) * (_previousRot.W * currentRot.X - _previousRot.X * currentRot.W - _previousRot.Y * currentRot.Z + _previousRot.Z * currentRot.Y);
	AngularVelocities.Y = (2 / dT) * (_previousRot.W * currentRot.Y + _previousRot.X * currentRot.Z - _previousRot.Y * currentRot.W - _previousRot.Z * currentRot.X);
	AngularVelocities.Z = (2 / dT) * (_previousRot.W * currentRot.Z - _previousRot.X * currentRot.Y + _previousRot.Y * currentRot.X - _previousRot.Z * currentRot.W);

	_previousRot = currentRot;

	return AngularVelocities;
}


FVector AIMUSensor::GetAcceleration(float dT)
{
	// Call the physics engine to get velocity in world frame, then divide by 100 to get m/s
	FVector currentVel = CapsuleComp->GetPhysicsLinearVelocity() / 100.0f;

	// Derive to get acceleration
	FVector Acceleration = (currentVel - _previousVel) / dT;

	// Add gravity
	//Acceleration += FVector(0.0f, 0.0f, -9.8f);

	// Get rotation and bring the acceleration vector in the IMU's local frame
	//FQuat currentRot = CapsuleComp->GetComponentRotation().Quaternion();
	//Acceleration = currentRot.RotateVector(Acceleration);

	// Update last velocity
	_previousVel = currentVel;

	return Acceleration;
}

FVector AIMUSensor::GetAcceleration(FQuat currentRot, float dT)
{
	// Call the physics engine to get velocity in world frame, then divide by 100 to get m/s
	FVector currentVel = CapsuleComp->GetPhysicsLinearVelocity() / 100.0f;

	// Derive to get acceleration
	FVector Acceleration = (currentVel - _previousVel) / dT;

	// Add gravity
	Acceleration += FVector(0.0f, 0.0f, -9.8f);

	// Get rotation and bring the acceleration vector in the IMU's local frame
	Acceleration = currentRot.RotateVector(Acceleration);

	// Update last velocity
	_previousVel = currentVel;

	return Acceleration;
}

float AIMUSensor::GetDT(float currentTime)
{
	// Compute difference, substitute old value and return diff
	float diff = currentTime - _lastIMUTstamp;
	_lastIMUTstamp = currentTime;

	return diff;
}




