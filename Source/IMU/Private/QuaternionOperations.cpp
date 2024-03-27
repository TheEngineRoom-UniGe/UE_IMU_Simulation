// Fill out your copyright notice in the Description page of Project Settings.

#include "QuaternionOperations.h"
#include <string>
#include "Engine/SkeletalMeshSocket.h"

using namespace std;

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

string UQuaternionOperations::QuaternionToStdString(FQuat rotation)
{
    return to_string(rotation.X) +
        "," +
        to_string(rotation.Y) +
        "," +
        to_string(rotation.Z) +
        "," +
        to_string(rotation.W);
        
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

//FQuat UQuaternionOperations::StringToQuaternion(FString poseString)
//{
//    TArray<FString> poseBuffer;
//    TArray<FString> locBuffer;
//    TArray<float> fLocBuffer;
//    TArray<FString> quatBuffer;
//    TArray<float> fQuatBuffer;
//    FQuat newQuat;
//
//    poseString.ParseIntoArray(poseBuffer, TEXT(","), true);
//
//    //location
//    poseBuffer[0].ParseIntoArray(locBuffer, TEXT(","), true);
//
//    for (FString locStr : locBuffer)
//    {
//        fLocBuffer.Add(FCString::Atof(*locStr));
//    }
//
//    for (FString quatStr : quatBuffer)
//    {
//        fQuatBuffer.Add(FCString::Atof(*quatStr));
//    }
//
//    newQuat.X = fQuatBuffer[0];
//    newQuat.Y = fQuatBuffer[1];
//    newQuat.Z = fQuatBuffer[2];
//    newQuat.W = fQuatBuffer[3];
//
//    return newQuat;
//}

void UQuaternionOperations::AddSocket(USkeletalMesh* skeleton)
{
    USkeletalMeshSocket* sock = NewObject<USkeletalMeshSocket>(skeleton);

    sock->BoneName = FName("head");
    sock->SocketName = FName("cazzoSocket");
    skeleton->AddSocket(sock, true);

    return;
}
