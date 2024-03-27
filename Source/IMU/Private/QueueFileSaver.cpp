 //Fill out your copyright notice in the Description page of Project Settings.


#include "QueueFileSaver.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../Public/QuaternionOperations.h"
#include "Components/PoseableMeshComponent.h"

using namespace std::chrono;

 //Sets default values for this component's properties
UQueueFileSaver::UQueueFileSaver()
{
	 //Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	 //off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SavingFrequency = 1.0f;

	StartTime = FDateTime::UtcNow();
}


// Called when the game starts
void UQueueFileSaver::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle animTimerHandle;
	FTimerHandle rotTimerHandle;
	FTimerHandle locTimerHandle;
	FTimerHandle accTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(animTimerHandle, this, &UQueueFileSaver::FileSaveAnimString, 1/SavingFrequency, true);
	GetWorld()->GetTimerManager().SetTimer(rotTimerHandle, this, &UQueueFileSaver::FileSaveRotString, 1 / SavingFrequency, true);
	GetWorld()->GetTimerManager().SetTimer(locTimerHandle, this, &UQueueFileSaver::FileSaveLocString, 1 / SavingFrequency, true);
	GetWorld()->GetTimerManager().SetTimer(accTimerHandle, this, &UQueueFileSaver::FileSaveAccString, 1 / SavingFrequency, true);
}


void UQueueFileSaver::FileSaveAnimString()
{
	TQueue<std::string>* AnimQ = &AnimationRecordingQueue;

	FString FolderPath = FPaths::ProjectDir() + "Dataset/" + ActionName;


	FString SkeletonSavingPath = FolderPath + "/" + BonesFilename;

	if (!std::filesystem::exists(TCHAR_TO_UTF8(*FolderPath))) {
		std::filesystem::create_directories(TCHAR_TO_UTF8(*FolderPath));
	}


	AsyncTask(ENamedThreads::AnyThread, [AnimQ, SkeletonSavingPath]()
		{
			std::string outStringAnim = "";
			std::ofstream animFile;
			animFile.open(TCHAR_TO_UTF8(*SkeletonSavingPath), std::ios_base::app);
			while (!AnimQ->IsEmpty())
			{
				AnimQ->Dequeue(outStringAnim);
				//FFileHelper::SaveStringToFile(outStringAnim, *SkeletonSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
				animFile << outStringAnim << std::endl;
			}
			animFile.close();
		});


	return;
}

void UQueueFileSaver::FileSaveRotString()
{
	TQueue<std::string>* IMUQ = &IMUOrientationRecordingQueue;

	FString FolderPath = FPaths::ProjectDir() + "Dataset/" + ActionName;


	FString IMUSavingPath = FolderPath + "/" + IMUFilename;

	if (!std::filesystem::exists(TCHAR_TO_UTF8(*FolderPath))) {
		std::filesystem::create_directories(TCHAR_TO_UTF8(*FolderPath));
	}

	AsyncTask(ENamedThreads::AnyThread, [IMUQ, IMUSavingPath]()
		{
			std::string outStringRot = "";
			std::ofstream rotFile;
			rotFile.open(TCHAR_TO_UTF8(*IMUSavingPath), std::ios_base::app);
			while (!IMUQ->IsEmpty())
			{
				IMUQ->Dequeue(outStringRot);
				//FFileHelper::SaveStringToFile(outStringRot, *IMUSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
				rotFile << outStringRot << std::endl;
			}
			rotFile.close();
		});


	return;
}

void UQueueFileSaver::FileSaveLocString()
{
	TQueue<std::string>* IMULoc = &IMULocationRecordingQueue;

	FString FolderPath = FPaths::ProjectDir() + "Dataset/" + ActionName;


	FString IMULocSavingPath = FolderPath + "/" + IMULocFilename;

	if (!std::filesystem::exists(TCHAR_TO_UTF8(*FolderPath))) {
		std::filesystem::create_directories(TCHAR_TO_UTF8(*FolderPath));
	}

	AsyncTask(ENamedThreads::AnyThread, [IMULoc, IMULocSavingPath]()
		{
			std::string outStringLoc = "";
			std::ofstream locFile;
			locFile.open(TCHAR_TO_UTF8(*IMULocSavingPath), std::ios_base::app);
			while (!IMULoc->IsEmpty())
			{
				IMULoc->Dequeue(outStringLoc);
				//FFileHelper::SaveStringToFile(TCHAR_TO_UTF8(*outStringLoc), *IMULocSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
				locFile << outStringLoc << std::endl;
			}
			locFile.close();
		});


	return;
}

void UQueueFileSaver::FileSaveAccString()
{
	TQueue<std::string>* IMUAcc = &IMUAccelerationRecordingQueue;

	FString FolderPath = FPaths::ProjectDir() + "Dataset/" + ActionName;


	FString IMUAccSavingPath = FolderPath + "/" + IMUAccFilename;

	if (!std::filesystem::exists(TCHAR_TO_UTF8(*FolderPath))) {
		std::filesystem::create_directories(TCHAR_TO_UTF8(*FolderPath));
	}

	AsyncTask(ENamedThreads::AnyThread, [IMUAcc, IMUAccSavingPath]()
		{
			std::string outStringAcc = "";
			std::ofstream accFile;
			accFile.open(TCHAR_TO_UTF8(*IMUAccSavingPath), std::ios_base::app);
			while (!IMUAcc->IsEmpty())
			{
				IMUAcc->Dequeue(outStringAcc);
				//FFileHelper::SaveStringToFile(TCHAR_TO_UTF8(*outStringLoc), *IMULocSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
				accFile << outStringAcc << std::endl;
			}
			accFile.close();
		});


	return;
}
//
//void UQueueFileSaver::FileSaveString()
//{
//	TQueue<std::string>* AnimQ = &AnimationRecordingQueue;
//	TQueue<std::string>* IMUQ = &IMUOrientationRecordingQueue;
//	TQueue<std::string>* IMULoc = &IMULocationRecordingQueue;
//	TQueue<std::string>* IMUAcc = &IMUAccelerationRecordingQueue;
//	TQueue<std::string>* IMUGyro = &IMUGyroscopeRecordingQueue;
//	TQueue<std::string>* IMUMag = &IMUMagnetometerRecordingQueue;
//
//	FString FolderPath = FPaths::ProjectDir() + "/Dataset/" + ActionName;
//
//
//	FString SkeletonSavingPath = FolderPath + "/" + "/" + BonesFilename;
//	FString IMUSavingPath = FolderPath + "/" + "/" + IMUFilename;
//	FString IMULocSavingPath = FolderPath + "/" + "/" + IMULocFilename;
//	FString IMUAccSavingPath = FolderPath + "/" + "/" + IMUAccFilename;
//	FString IMUGyroSavingPath = FolderPath + "/" + "/" + IMUGyroFilename;
//	FString IMUMagSavingPath = FolderPath + "/" + "/" + IMUMagFilename;
//
//	AsyncTask(ENamedThreads::AnyThread, [AnimQ,  SkeletonSavingPath]()
//	{
//		std::string outStringAnim = "";
//		std::ofstream animFile;
//		animFile.open(TCHAR_TO_UTF8(*SkeletonSavingPath), std::ios_base::app);
//		while (!AnimQ->IsEmpty())
//		{
//			AnimQ->Dequeue(outStringAnim);
//			//FFileHelper::SaveStringToFile(outStringAnim, *SkeletonSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//			animFile << outStringAnim << std::endl;
//		}
//		animFile.close();
//	});
//
//	AsyncTask(ENamedThreads::AnyThread, [IMUQ, IMUSavingPath]()
//	{
//		std::string outStringRot = "";
//		std::ofstream rotFile;
//		rotFile.open(TCHAR_TO_UTF8(*IMUSavingPath), std::ios_base::app);
//		while (!IMUQ->IsEmpty())
//		{
//			IMUQ->Dequeue(outStringRot);
//			//FFileHelper::SaveStringToFile(outStringRot, *IMUSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//			rotFile << outStringRot << std::endl;
//		}
//		rotFile.close();
//	});
//
//
//	AsyncTask(ENamedThreads::AnyThread, [IMULoc, IMULocSavingPath]()
//	{
//		std::string outStringLoc = "";
//		std::ofstream locFile;
//		locFile.open(TCHAR_TO_UTF8(*IMULocSavingPath), std::ios_base::app);
//		while (!IMULoc->IsEmpty())
//		{
//			IMULoc->Dequeue(outStringLoc);
//			//FFileHelper::SaveStringToFile(TCHAR_TO_UTF8(*outStringLoc), *IMULocSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//			locFile << outStringLoc << std::endl;
//		}
//		locFile.close();
//	});
//
//
//	//AsyncTask(ENamedThreads::AnyThread, [AnimQ, IMUQ, IMULoc, IMUAcc, IMUGyro, IMUMag, SkeletonSavingPath, IMUSavingPath, IMULocSavingPath, IMUAccSavingPath, IMUGyroSavingPath, IMUMagSavingPath]()
//	//	{
//	//		//int tries = 0;
//	//		//int tries2 = 0;
//	//		// This code will run asynchronously, without freezing the game thread
//	//		FString outStringAnim = "";
//	//		FString outStringRot = "";
//	//		FString outStringLoc = "";
//
//	//		while (AnimQ->Dequeue(outStringAnim))
//	//		{
//
//	//			//UE_LOG(LogTemp, Warning, TEXT("%s"), *outString);
//	//			//UE_LOG(LogTemp, Warning, TEXT("AsyncTask, %s"), *FString::FromInt(tries));
//	//			FFileHelper::SaveStringToFile(outStringAnim, *SkeletonSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//	//			//tries++;
//	//		}
//
//	//		while (IMUQ->Dequeue(outStringRot))
//	//		{
//	//			//UE_LOG(LogTemp, Warning, TEXT("AsyncTask, %s"), *FString::FromInt(tries2));
//	//			//UE_LOG(LogTemp, Warning, TEXT("%s"), *outString);
//	//			FFileHelper::SaveStringToFile(outStringRot, *IMUSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//	//			//tries2++;
//	//		}
//
//	//		while (IMULoc->Dequeue(outStringLoc))
//	//		{
//	//			//UE_LOG(LogTemp, Warning, TEXT("%s"), *outString);
//	//			FFileHelper::SaveStringToFile(outStringLoc, *IMULocSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//	//			//tries2++;
//	//		}
//
//	//		//while (IMUAcc->Dequeue(outString))
//	//		//{
//	//		//	FFileHelper::SaveStringToFile(outString, *IMUAccSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//	//		//	//tries2++;
//	//		//}
//
//	//		//while (IMUGyro->Dequeue(outString))
//	//		//{
//	//		//	FFileHelper::SaveStringToFile(outString, *IMUGyroSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//	//		//	//tries2++;
//	//		//}
//
//	//		//while (IMUMag->Dequeue(outString))
//	//		//{
//	//		//	FFileHelper::SaveStringToFile(outString, *IMUMagSavingPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
//	//		//	//tries2++;
//	//		//}
//	//	});
//	return;
//}

bool UQueueFileSaver::ReadAnimationFile()
{
	FString savingPath = FPaths::ProjectDir() + "/Dataset/" + ActionName + "/" + "/" + BonesFilename;
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

	TArray<FString> headerBuffer;

	for (int i=0; i<tempArray.Num(); i++)
	{
		FAnimationLine newAnimationLine;
		TArray<FString> lineBuffer;


		if (i == 0) 
		{
			tempArray[0].ParseIntoArray(headerBuffer, TEXT(";"), true);
			// discard the "timestamp" field
			headerBuffer.RemoveAt(0);
		}

		else
		{
			tempArray[i].ParseIntoArray(lineBuffer, TEXT(";"), true);

			newAnimationLine.Timestamp = FCString::Atod(*lineBuffer[0]);
			lineBuffer.RemoveAt(0);

			for (int j = 0; j<lineBuffer.Num(); j++)
			{
				FBoneOrientation newBoneInfo;

				newBoneInfo.BoneName = headerBuffer[j];
				if (headerBuffer[j].Contains("root")) {
					// root has both location and orientation, let's discard location
					TArray<FString> poseBuffer;
					lineBuffer[j].ParseIntoArray(poseBuffer, TEXT("|"), true);
					lineBuffer[j] = poseBuffer[0];
				}
				newBoneInfo.Orientation = UQuaternionOperations::StringToQuaternion(lineBuffer[j]);

				newAnimationLine.BoneInfoArray.Add(newBoneInfo);
			}

			AnimationPlaybackQueue.Enqueue(newAnimationLine);
		}
	}

	return;
}

void UQueueFileSaver::EnqueueAnimationString(std::string newTxtSkeletonPose)
{
	if (AnimationRecordingQueue.Enqueue(newTxtSkeletonPose))
	{
		return;
	}
}

void UQueueFileSaver::EnqueueIMUString(std::string newTxtIMUOrientation)
{
	if (IMUOrientationRecordingQueue.Enqueue(newTxtIMUOrientation))
	{
		return;
	}
}

void UQueueFileSaver::EnqueueIMULocString(std::string newTxtIMULocation)
{
	if (IMULocationRecordingQueue.Enqueue(newTxtIMULocation))
	{
		return;
	}
}

void UQueueFileSaver::EnqueueIMUAccString(std::string newTxtIMUAcceleration)
{
	if (IMUAccelerationRecordingQueue.Enqueue(newTxtIMUAcceleration))
	{
		return;
	}
}

void UQueueFileSaver::EnqueueIMUGyroString(std::string newTxtIMUGyroscope)
{
	if (IMUGyroscopeRecordingQueue.Enqueue(newTxtIMUGyroscope))
	{
		return;
	}
}

void UQueueFileSaver::EnqueueIMUMagString(std::string newTxtIMUMagnetometer)
{
	if (IMUMagnetometerRecordingQueue.Enqueue(newTxtIMUMagnetometer))
	{
		return;
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

double UQueueFileSaver::GetTimestamp() {

	FTimespan ts = FDateTime::UtcNow() - StartTime;

	return ts.GetTotalMilliseconds()/1000.0f;
}

FDateTime UQueueFileSaver::ResetStartTime() {

	StartTime = FDateTime::UtcNow();

	return StartTime;
}

