// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomAssetValidator.h"
#include "Textures/SlateIcon.h"
#include "Engine/Texture2D.h"
#include "SlateNotificationsBPLibrary.h"
#include "Misc/DataValidation.h"

UCustomAssetValidator::UCustomAssetValidator()
{
	bIsEnabled = true;
}

bool UCustomAssetValidator::CanValidateAsset_Implementation(UObject* InAsset) const
{
	return InAsset && InAsset->IsA(UTexture2D::StaticClass());
	
}

EDataValidationResult UCustomAssetValidator::ValidateLoadedAsset_Implementation(UObject* InAsset,
	TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = EDataValidationResult::Valid;
	
	if (UTexture2D* Texture = Cast<UTexture2D>(InAsset))
	{
		FString AssetName = Texture->GetName();
		if (!AssetName.StartsWith(TEXT("T_")))
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Asset name must start with 'T_'. Current: %s"), *AssetName)));
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		}

		if (Texture->GetSizeX() != 1024 || Texture->GetSizeY() != 1024)
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Texture must be 1024x1024. Current: %dx%d"), Texture->GetSizeX(), Texture->GetSizeY())));
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		}
	}
	
	if (Result == EDataValidationResult::Valid)
	{
		USlateNotificationsBPLibrary::NotifySuccess(FText::FromString(TEXT("All the assets are fine")));
	}
	else
	{
		USlateNotificationsBPLibrary::NotifyError(FText::FromString(TEXT("Asset validation failed")), TEXT("https://yourproject.com/asset-requirements"), FText::FromString(TEXT("View Requirements")));
	}
	return Result;
}
