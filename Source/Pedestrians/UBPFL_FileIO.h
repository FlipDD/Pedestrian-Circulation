// Copyright © 2020 Filipe Ribeiro, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "UBPFL_FileIO.generated.h"

/**
 * 
 */
UCLASS()
class PEDESTRIANS_API UUBPFL_FileIO : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, Category = "File IO")
		static FString LoadFileToString(FString Filename);

		UFUNCTION(BlueprintCallable, Category = "File IO")
		static TArray<FString> LoadFileToStringArray(FString Filename);

		UFUNCTION(BlueprintCallable, Category = "File IO")
		static void SaveStringToFile(TArray<FString> DataToSave, FString Filename);

		UFUNCTION(BlueprintCallable, Category = "File IO")
		static void CreateFile(FString Filename);		
};
