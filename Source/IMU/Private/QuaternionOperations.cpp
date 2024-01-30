// Fill out your copyright notice in the Description page of Project Settings.


#include "QuaternionOperations.h"

FQuat UQuaternionOperations::RotatorToQuaternion(FRotator rotation)
{
    return rotation.Quaternion();
}

FRotator UQuaternionOperations::QuaternionToRotator(FQuat rotation)
{
    return rotation.Rotator();
}

FString UQuaternionOperations::QuaternionToString(FQuat rotation)
{
    return FString::SanitizeFloat(rotation.X) +
    FString(",") +
    FString::SanitizeFloat(rotation.Y) +
    FString(",") +
    FString::SanitizeFloat(rotation.Z) +
    FString(",") +
    FString::SanitizeFloat(rotation.W);
}

FQuat UQuaternionOperations::StringToQuaternion(FString quatString)
{
    TArray<FString> strBuffer;
    TArray<float> fBuffer;
    FQuat newQuat;

    quatString.ParseIntoArray(strBuffer, TEXT(","), true);

    for (FString floatStr : strBuffer)
    {
        fBuffer.Add(FCString::Atof(*floatStr));
    }

    newQuat.X = fBuffer[0];
    newQuat.Y = fBuffer[1];
    newQuat.Z = fBuffer[2];
    newQuat.W = fBuffer[3];

    return newQuat;
}
