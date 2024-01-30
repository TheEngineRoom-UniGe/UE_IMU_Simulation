// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <chrono>
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

protected:

	UPROPERTY(EditDefaultsOnly, Category="Saving")
	float SavingFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	int Trial;

	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString BonesFilename;

	UPROPERTY(EditDefaultsOnly, Category = "Saving")
	FString IMUFilename;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Saving")
	FString cr = LINE_TERMINATOR;

	TQueue<FString> AnimationRecordingQueue;
	TQueue<FAnimationLine> AnimationPlaybackQueue;
	TQueue<FString> IMUOrientationRecordingQueue;

	// Called when the game starts
	virtual void BeginPlay() override;

	void FileSaveString();

	UFUNCTION(BlueprintCallable)
	void EnqueueAnimationString(FString newTxtSkeletonPose);

	UFUNCTION(BlueprintCallable)
	void EnqueueIMUString(FString newTxtIMUOrientation);

	UFUNCTION(BlueprintCallable)
	FAnimationLine DequeueFString();

	UFUNCTION(BlueprintCallable)
	bool ReadAnimationFile();

	void ParseAndLoadAnimationFile(FString buffer);

	UFUNCTION(BlueprintCallable)
	float CurrentTimeMillis();

private:
	std::chrono::high_resolution_clock::time_point startTime;

};


