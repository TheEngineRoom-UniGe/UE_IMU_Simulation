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
    //FString IMURotLine = FString::SanitizeFloat(TStamp) + FString(";");
    //FString IMULocLine = FString::SanitizeFloat(TStamp) + FString(";");
    //FString IMUAccLine = FString::SanitizeFloat(TStamp) + FString(";");
    //FString IMUGyrLine = FString::SanitizeFloat(TStamp) + FString(";");
    //FString IMUMagLine = FString::SanitizeFloat(TStamp) + FString(";");

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

            FQuat tmpIMUQuat = imu->GetActorRotation().Quaternion();
            tmpIMUQuat.Y *= -1.0;
            // Append Orientation
            //IMURotLine.Append(
            //    UQuaternionOperations::QuaternionToString(tmpIMUQuat) + FString(";")
            //);
            IMURotLine += UQuaternionOperations::QuaternionToStdString(tmpIMUQuat) + ";";
        
            FVector tmpIMUloc = imu->GetActorLocation();
            // Append Location
   /*         IMULocLine.Append(
                LocationToString(tmpIMUloc) + FString(";")
            );*/

            IMULocLine += LocationToStdString(tmpIMUloc) + ";";

            //// Append Acceleration
            //IMUAccLine.Append(
            //    LocationToString(imu->GetAcceleration(tmpIMUQuat, dT))  + FString(";")
            //);
            IMUAccLine += LocationToStdString(imu->GetAcceleration(tmpIMUQuat, dT)) + ";";

            //// Append Angular Velocity
            //IMUGyrLine.Append(
            //    LocationToString(imu->GetAngularVelocity(tmpIMUQuat, dT)) + FString(";")
            //);

            //// Append Magnetic Field
            //IMUMagLine.Append(
            //    LocationToString(imu->GetMagneticField(tmpIMUQuat)) + FString(";")
            //);

        }
    }

    // Remove last ;
    IMURotLine.pop_back();
    IMULocLine.pop_back();
    IMUAccLine.pop_back();

    //IMURotLine.RemoveAt(IMURotLine.Len() - 1);
    //IMULocLine.RemoveAt(IMURotLine.Len() - 1);
    //IMUAccLine.RemoveAt(IMURotLine.Len() - 1);
    //IMUGyrLine.RemoveAt(IMURotLine.Len() - 1);
    //IMUMagLine.RemoveAt(IMURotLine.Len() - 1);

    // Insert Line Terminator
    //IMURotLine.Append(LINE_TERMINATOR);
    //IMULocLine.Append(LINE_TERMINATOR);
    //IMUAccLine.Append(LINE_TERMINATOR);
    //IMUGyrLine.Append(LINE_TERMINATOR);
    //IMUMagLine.Append(LINE_TERMINATOR);

    //FString strArr[] =  {
    //                        IMURotLine,
    //                        IMULocLine,
    //                        IMUAccLine,
    //                        IMUGyrLine,
    //                        IMUMagLine
    //                    };

    /*TArray<FString> TstrArr;*/
    //TstrArr.Append(strArr, UE_ARRAY_COUNT(strArr));

    qFileSaver->EnqueueIMUString(IMURotLine);
    qFileSaver->EnqueueIMULocString(IMULocLine);
    qFileSaver->EnqueueIMUAccString(IMUAccLine);
    //qFileSaver->EnqueueIMUGyroString(IMUGyrLine);
    //qFileSaver->EnqueueIMUMagString(IMUMagLine);

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