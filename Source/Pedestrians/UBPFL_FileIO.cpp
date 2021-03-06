// Copyright © 2020 Filipe Ribeiro, All Rights Reserved.


#include "UBPFL_FileIO.h"
#include "Misc/Paths.h"
#include "HAL\PlatformFilemanager.h"


FString UUBPFL_FileIO::LoadFileToString(FString Filename)
{
	FString directory = FPaths::GameContentDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename;
		FFileHelper::LoadFileToString(result, *myFile);
	}
	return result;
}

TArray<FString> UUBPFL_FileIO::LoadFileToStringArray(FString Filename)
{
	FString directory = FPaths::GameContentDir();
	TArray<FString> result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename;
		FFileHelper::LoadFileToStringArray(result, *myFile);
	}
	return result;
}

void UUBPFL_FileIO::SaveStringToFile(TArray<FString> DataToSave, FString Filename)
{
	FString directory = FPaths::GameContentDir();
	TArray<FString> result;

	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + Filename + ".txt";
		FFileHelper::SaveStringArrayToFile(DataToSave, *myFile);
	}
}

void UUBPFL_FileIO::CreateFile(FString Filename)
{

}
