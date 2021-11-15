// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
// Sets default values
ADialogueActor::ADialogueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USoundWave> SWBattleBGM(TEXT("SoundWave'/Game/wavs/xiongxiong.xiongxiong'"));

	base = SWBattleBGM.Object;
}

// Called when the game starts or when spawned
void ADialogueActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADialogueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ADialogueActor::test()
{
	return GetNextDialogue(10);
}

int ADialogueActor::test1(USoundBase* inData, USoundBase* outData)
{
	return GetNextDialogue(inData, outData);
}

USoundBase* ADialogueActor::test2(USoundBase* inData)
{
	if (base)
	{
		
		UGameplayStatics::PlaySound2D(GetWorld(), LoadWaveDataFromFile(TEXT("C:\\Users\\MI\\Downloads\\xiongxiong.wav")));
	}
	return GetNextDialogue(inData);
}

USoundWave* ADialogueActor::LoadWaveDataFromFile(const FString& FilePath)
{
	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return nullptr;


	TArray < uint8 > rawFile;

	FFileHelper::LoadFileToArray(rawFile, FilePath.GetCharArray().GetData());
	FWaveModInfo WaveInfo;

	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num()))
	{
		sw->InvalidateCompressedData();

		sw->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = sw->RawData.Realloc(rawFile.Num());
	//	void* LockedData = sw->RawData.Realloc(((USoundWave*)base)->RawData.GetBulkDataSize());
	//	((USoundWave*)base)->RawData.GetCopy(&LockedData);
		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
		sw->RawData.Unlock();

		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
		if (DurationDiv)
		{
			sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
		}
		else
		{
			sw->Duration = 0.0f;
		}

		sw->SetSampleRate(*WaveInfo.pSamplesPerSec);
		sw->NumChannels = *WaveInfo.pChannels;
		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	}
	else {
		return nullptr;
	}

	return sw;
}