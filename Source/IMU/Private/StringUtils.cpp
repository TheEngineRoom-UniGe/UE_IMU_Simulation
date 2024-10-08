// Fill out your copyright notice in the Description page of Project Settings.


#include "StringUtils.h"
#include <string>
#include "LocalPoseableMeshComponent.h"
#include "QueueFileSaver.h"
#include "QuaternionOperations.h"
#include "IMUSensor.h"

using namespace std;

void UStringUtils::CreateAndEnqueueAnimHeader(TArray<FName> BoneList, UQueueFileSaver* qFileSaver)
{
    string header = "Timestamp;";

    for (FName name : BoneList)
    {
        string tmpStr = string(TCHAR_TO_UTF8(*name.ToString()));
        FString nameFStr;
        name.ToString(nameFStr);

        // Check if the string contains "socket"
        if (!nameFStr.Contains(FString("socket"), ESearchCase::IgnoreCase))
        {
            tmpStr += ";";
            // if not append it to header
            header += tmpStr;
        }
    }

    // Remove last ;
    header.pop_back();

    // Insert Line Terminator
    //header.Append(LINE_TERMINATOR);
    qFileSaver->EnqueueAnimationString(header);

    return;
}

void UStringUtils::CreateAndEnqueueIMUHeader(TArray<AActor*> IMUList, UQueueFileSaver* qFileSaver)
{
    string header = "Timestamp;";

    for (AActor* imu : IMUList)
    {
        string tmpStr = string(TCHAR_TO_UTF8(*imu->Tags[0].ToString())) + ";";

        header += tmpStr;
    }

    // Remove last ;
    header.pop_back();

    // Insert Line Terminator
    //header.Append(LINE_TERMINATOR);
    qFileSaver->EnqueueIMUString(header);
    qFileSaver->EnqueueIMULocString(header);
    qFileSaver->EnqueueIMUAccString(header);
    qFileSaver->EnqueueIMUGyroString(header);
    qFileSaver->EnqueueIMUMagString(header);

    return;
}

void UStringUtils::CreateAndEnqueueAnimLine(TArray<FName> BoneList, ULocalPoseableMeshComponent* PosMesh, float TStamp, UQueueFileSaver* qFileSaver)
{
    // Initialize the animation line to the timestamp + ;
    //FString animLine = FString::SanitizeFloat(TStamp) + FString(";");
    string animLine = to_string(TStamp) + ";";


    for (FName name : BoneList)
    {
        string tmpStr = "";
        FString nameFStr;
        name.ToString(nameFStr);

        // Check if the string contains "socket"
        if (!nameFStr.Contains(FString("socket"), ESearchCase::IgnoreCase))
        {            
            if (nameFStr.Contains(FString("pelvis"), ESearchCase::IgnoreCase))
            {
                // The pelvis should be the root motion and we save orientation and location with this format: "R.X,R.Y,R.Z,R.W|L.X,L.Y,L.Z;"
                //tmpStr = UQuaternionOperations::QuaternionToString(PosMesh->GetBoneRotationByName(name, EBoneSpaces::WorldSpace).Quaternion()) +
                //         FString("|") +
                //         LocationToString(PosMesh->GetBoneLocationByName(name, EBoneSpaces::WorldSpace)) +
                //         FString(";");
                tmpStr = UQuaternionOperations::QuaternionToStdString(PosMesh->GetBoneRotationByName(name, EBoneSpaces::WorldSpace).Quaternion()) +
                         "|" +
                         LocationToStdString(PosMesh->GetBoneLocationByName(name, EBoneSpaces::WorldSpace)) +
                         ";";
            }
            else 
            {
                // Get Local Rotation by Bone Name and convert it to String format is: "X,Y,Z,W;"
                //tmpStr = UQuaternionOperations::QuaternionToString(PosMesh->GetBoneLocalRotationByName(name)) + FString(";");
                tmpStr = UQuaternionOperations::QuaternionToStdString(PosMesh->GetBoneLocalRotationByName(name)) + ";";
                // if not append it to header
            }
        }

        animLine += tmpStr;
        //animLine.Append(tmpStr);
    }

    // Remove last ;
    animLine.pop_back();
    //animLine.RemoveAt(animLine.Len() - 1);

    // Insert Line Terminator
    //animLine.Append(LINE_TERMINATOR);

    qFileSaver->EnqueueAnimationString(animLine);

    return;
}


void UStringUtils::CreateAndEnqueueIMULine(TArray<AActor*> IMUList, float TStamp, UQueueFileSaver* qFileSaver)
{
    // Initialize the animation line to the timestamp + ;

    string IMURotLine = to_string(TStamp) + ";";
    string IMULocLine = to_string(TStamp) + ";";
    string IMUAccLine = to_string(TStamp) + ";";
    string IMUGyrLine = to_string(TStamp) + ";";
    string IMUMagLine = to_string(TStamp) + ";";

    for (AActor* imuActor : IMUList)
    {
        // Cast to AIMUSensor
        AIMUSensor* imu = Cast<AIMUSensor>(imuActor);

        if (ensure(imu)) {

            // Compute DeltaTime  
            float dT = imu->GetDT(TStamp);

            // Need to put a minus on Y to get to right-handed system
            FQuat tmpIMUQuat = imu->GetCapsuleAbsoluteRotation().Quaternion();
            tmpIMUQuat.Y *= -1.0f;
            IMURotLine += UQuaternionOperations::QuaternionToStdString(tmpIMUQuat) + ";";
             
            // Need to put a minus on Y to get to right-handed system
            FVector tmpIMUloc = imu->GetActorLocation();
            tmpIMUloc.Y *= -1.0f;
            IMULocLine += LocationToStdString(tmpIMUloc) + ";";

            // Need to put a minus on Y to get to right-handed system
            FVector tmpAcc = imu->GetAcceleration(dT);
            tmpAcc.Y *= -1.0f;
            IMUAccLine += LocationToStdString(tmpAcc) + ";";

            // Need to put minus on Y to get to right-handed system
            // Need to flip all rotations because they don't follow right-hand rule
            // Result is minus on X and Z 
            FVector tmpGyr = imu->GetAngularVelocity();
            tmpGyr.X *= -1.0f;
            tmpGyr.Z *= -1.0f;
            IMUGyrLine += LocationToStdString(tmpGyr) + ";";

            // Need to put minus on Y to get to right-handed system, then need to put minus on Y and Z because the magnetometer has different reference on the sensor
            // The result is just a minus on Z
            FVector tmpMag = imu->GetMagneticField();
            tmpMag.Z *= -1.0f;
            IMUMagLine += LocationToStdString(tmpMag) + ";";

        }
    }

    // Remove last ";"
    IMURotLine.pop_back();
    IMULocLine.pop_back();
    IMUAccLine.pop_back();
    IMUGyrLine.pop_back();
    IMUMagLine.pop_back();

    // Enqueue strings
    qFileSaver->EnqueueIMUString(IMURotLine);
    qFileSaver->EnqueueIMULocString(IMULocLine);
    qFileSaver->EnqueueIMUAccString(IMUAccLine);
    qFileSaver->EnqueueIMUGyroString(IMUGyrLine);
    qFileSaver->EnqueueIMUMagString(IMUMagLine);
    
    return;
}


FString UStringUtils::LocationToString(FVector loc)
{
    return FString::SanitizeFloat(loc.X) +
        FString(",") +
        FString::SanitizeFloat(loc.Y) +
        FString(",") +
        FString::SanitizeFloat(loc.Z);
}

string UStringUtils::LocationToStdString(FVector loc)
{
    return to_string(loc.X) +
        "," +
        to_string(loc.Y) +
        "," +
        to_string(loc.Z);

}

const FString UStringUtils::GetAnimName(FAssetData asset) {
    return asset.AssetName.ToString();
}