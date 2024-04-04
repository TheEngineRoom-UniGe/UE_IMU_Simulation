// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <chrono>
#include <string>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QueueFileSaver.generated.h"

//DECLARE_DYNAMIC_DELEGATE_OneParam(FAsyncDelegateFileWrite, const TArray<float>&, OutData);

USTRUCT(BlueprintType)
struct FBoneOrientation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat Orientation;

};


USTRUCT(BlueprintType)
struct FAnimationLine
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Timestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBoneOrientation> BoneInfoArray;

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UQueueFileSaver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQueueFileSaver();

	void EnqueueAnimationString(std::string newTxtSkeletonPose);

	void EnqueueIMUString(std::string newTxtIMUOrientation);

	void EnqueueIMULocString(std::string newTxtIMULocation);

	void EnqueueIMUAccString(std::string newTxtIMUAcceleration);

	void EnqueueIMUGyroString(std::string newTxtIMUGyroscope);

	void EnqueueIMUMagString(std::string newTxtIMUMagnetometer);


protected:

	FDateTime StartTime;

	UPROPERTY(EditDefaultsOnly, Category="Saving")
	float SavingFrequency;

	//UPROPERTY(EditDefaultsOnly, Category = "Saving")
	//int Trial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving")
	FString ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString BonesFilename;

	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString IMUFilename;
	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString IMULocFilename;
	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString IMUAccFilename;
	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString IMUGyroFilename;
	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString IMUMagFilename;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Saving")
	FString cr = LINE_TERMINATOR;

	TQueue<std::string> AnimationRecordingQueue;
	TQueue<FAnimationLine> AnimationPlaybackQueue;
	TQueue<std::string> IMUOrientationRecordingQueue;
	TQueue<std::string> IMULocationRecordingQueue;
	TQueue<std::string> IMUAccelerationRecordingQueue;
	TQueue<std::string> IMUGyroscopeRecordingQueue;
	TQueue<std::string> IMUMagnetometerRecordingQueue;

	// Called when the game starts
	virtual void BeginPlay() override;

	void FileSaveAnimString();
	void FileSaveRotString();
	void FileSaveLocString();
	void FileSaveAccString();
	void FileSaveGyrString();
	void FileSaveMagString();



	//void FileSaveString();

	UFUNCTION(BlueprintCallable)
	FAnimationLine DequeueFString();

	UFUNCTION(BlueprintCallable)
	bool ReadAnimationFile();

	void ParseAndLoadAnimationFile(FString buffer);

	UFUNCTION(BlueprintCallable)
	double GetTimestamp();

	UFUNCTION(BlueprintCallable)
	FDateTime ResetStartTime();

	//UFUNCTION(BlueprintCallable)
	//FString CreateAnimationLine(TArray<FName> SockNames, UPoseableMeshComponent* Mesh, double TimeStamp);

private:

};


