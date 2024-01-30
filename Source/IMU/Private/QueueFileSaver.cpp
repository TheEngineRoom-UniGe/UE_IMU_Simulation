 //Fill out your copyright notice in the Description page of Project Settings.


#include "QueueFileSaver.h"
#include "../Public/QuaternionOperations.h"

using namespace std::chrono;

 //Sets default values for this component's properties
UQueueFileSaver::UQueueFileSaver()
{
	 //Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	 //off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SavingFrequency = 10.0f;

	startTime = high_resolution_clock::now();
}


// Called when the game starts
void UQueueFileSaver::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle timerHandle;

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UQueueFileSaver::FileSaveString, 1/SavingFrequency, true);

}

float UQueueFileSaver::CurrentTimeMillis()
{
	high_resolution_clock::time_point newTime = high_resolution_clock::now();
	duration<float, std::milli> time_span = newTime - startTime;
	return time_span.count();
}


void UQueueFileSaver::FileSaveString()
{
	TQueue<FString>* AnimQ = &AnimationRecordingQueue;
	TQueue<FString>* IMUQ = &IMUOrientationRecordingQueue;
	FString SkeletonSavingPath = FPaths::ProjectDir() + "/" + ActionName + "/" + FString::FromInt(Trial) + "/" + BonesFilename;
	FString IMUSavingPath = FPaths::ProjectDir() + "/" + ActionName + "/" + FString::FromInt(Trial) + "/" + IMUFilename;

	AsyncTask(ENamedThreads::AnyThread, [AnimQ, IMUQ, SkeletonSavingPath, IMUSavingPath]()
		{
			int tries = 0;
			int tries2 = 0;
			// This code will run asynchronously, without freezing the game thread
			FString outString;

			while (AnimQ->Dequeue(outString))
			{
				UE_LOG(LogTemp, Warning, TEXT("AsyncTask, %s"), *FString::FromInt(tries));
				UE_LOG(LogTemp, Warning, TEXT("%s"), *outString);
				FFileHelper::SaveStringToFile(outString, *SkeletonSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
				tries++;
			}

			while (IMUQ->Dequeue(outString))
			{
				UE_LOG(LogTemp, Warning, TEXT("AsyncTask, %s"), *FString::FromInt(tries2));
				UE_LOG(LogTemp, Warning, TEXT("%s"), *outString);
				FFileHelper::SaveStringToFile(outString, *IMUSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
				tries2++;
			}
		});
	return;
}

bool UQueueFileSaver::ReadAnimationFile()
{
	FString savingPath = FPaths::ProjectDir() + "/" + ActionName + "/" + FString::FromInt(Trial) + "/" + BonesFilename;
	FString buffer;

	//AsyncTask(ENamedThreads::AnyThread, [&buffer, savingPath]()
	//	{
	//		if (!FFileHelper::LoadFileToString(buffer, *savingPath)) {
	//			return false;
	//		}

		//});
	if (!FFileHelper::LoadFileToString(buffer, *savingPath)) {
		return false;
	}
	ParseAndLoadAnimationFile(buffer);

	return true;
}

void UQueueFileSaver::ParseAndLoadAnimationFile(FString buffer)
{
	TArray<FString> tempArray;
	buffer.ParseIntoArray(tempArray, LINE_TERMINATOR, true);

	for (FString line : tempArray)
	{
		FAnimationLine newAnimationLine;
		TArray<FString> lineBuffer;

		line.ParseIntoArray(lineBuffer, TEXT("#"), true);

		newAnimationLine.Timestamp = FCString::Atod(*lineBuffer[0]);
		lineBuffer.RemoveAt(0);

		for (FString boneInfo : lineBuffer)
		{
			FBoneOrientation newBoneInfo;
			TArray<FString> boneInfoBuffer;

			boneInfo.ParseIntoArray(boneInfoBuffer, TEXT(";"), true);

			newBoneInfo.BoneName = boneInfoBuffer[0];
			newBoneInfo.Orientation = UQuaternionOperations::StringToQuaternion(boneInfoBuffer[1]);

			newAnimationLine.BoneInfoArray.Add(newBoneInfo);
		}

		AnimationPlaybackQueue.Enqueue(newAnimationLine);
	}

	return;
}

void UQueueFileSaver::EnqueueAnimationString(FString newTxtSkeletonPose)
{
	if (AnimationRecordingQueue.Enqueue(newTxtSkeletonPose))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnqueueFString"));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *string.to);
	}
}

void UQueueFileSaver::EnqueueIMUString(FString newTxtIMUOrientation)
{
	if (IMUOrientationRecordingQueue.Enqueue(newTxtIMUOrientation))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnqueueFString"));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *string.to);
	}
}

FAnimationLine UQueueFileSaver::DequeueFString()
{
	FAnimationLine outAnimLine;
	if (AnimationPlaybackQueue.Dequeue(outAnimLine))
	{
		return outAnimLine;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Error in dequeueing the Animation line"));
	outAnimLine.Timestamp = -1.0f;
	return outAnimLine;
}
