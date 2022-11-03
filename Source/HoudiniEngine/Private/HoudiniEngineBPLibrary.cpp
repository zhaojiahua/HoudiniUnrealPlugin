// Copyright Epic Games, Inc. All Rights Reserved.

#include "HoudiniEngineBPLibrary.h"
#include "HoudiniEngine.h"

UHoudiniEngineBPLibrary::UHoudiniEngineBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UHoudiniEngineBPLibrary::HoudiniEngineSampleFunction(float Param)
{
	return -1;
}

FHoudiniSession UHoudiniEngineBPLibrary::StartServerAndCreateSession(FHoudiniSession inSession)
{
	HAPI_Session  houdiniSession = inSession.ToHAPI_Session();
	HAPI_Result createResult = HAPI_CreateThriftNamedPipeSession(&houdiniSession, "hapi");
	if (createResult != HAPI_Result::HAPI_RESULT_SUCCESS)
	{
		HAPI_ThriftServerOptions serverOptions{ 0 };
		serverOptions.autoClose = true;
		serverOptions.timeoutMs = 3000.0f;
		HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr);
		UE_LOG(LogTemp, Warning, TEXT("Houdini named pipe server has been successfully started!"));
		HAPI_CreateThriftNamedPipeSession(&houdiniSession, "hapi");
	}
	UE_LOG(LogTemp, Warning, TEXT("Houdini named pipe server has been successfully created!"));
	return FHoudiniSession::FromHAPI_Session(houdiniSession);
}

FHoudiniCookOption UHoudiniEngineBPLibrary::CreateHoudiniCookOption()
{
	HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
	FHoudiniCookOption tempHoudiniCookOption;
	tempHoudiniCookOption.houdiniCooOption = cookOptions;
	return tempHoudiniCookOption;
}

void UHoudiniEngineBPLibrary::InitializeHoudiniSession(FHoudiniSession inhoudiniSession,const FHoudiniCookOption& inhoudiniCookOption)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return;	//首先检查houdiniSession是否合法
	HAPI_Session hapi_session = inhoudiniSession.ToHAPI_Session();
	HAPI_Initialize(&hapi_session, &inhoudiniCookOption.houdiniCooOption, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool UHoudiniEngineBPLibrary::HoudiniSessionIsValid(FHoudiniSession inhoudiniSession)
{
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_IsSessionValid(&houSession);
	return tempResult == HAPI_Result::HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniLoadAssetLibraryFromFile(FHoudiniSession inhoudiniSession, FString otlFilePath, bool allowOverWrite, int& assetLibraryId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_AssetLibraryId assetLibId = -1;
	HAPI_Result tempResult = HAPI_LoadAssetLibraryFromFile(&houSession, TCHAR_TO_UTF8(*otlFilePath), (HAPI_Bool)allowOverWrite, &assetLibId);
	assetLibraryId = (int)assetLibId;
	return tempResult == HAPI_Result::HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::GetAvailableAssetOptionName(FHoudiniSession inhoudiniSession, int inlibrary_id, FString& fullOptionName, FString& label)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	int assetsCount = INDEX_NONE;
	HAPI_GetAvailableAssetCount(&houSession, (HAPI_AssetLibraryId)inlibrary_id, &assetsCount);
	check(assetsCount == 1);	//只能给一个资产,多了就会崩溃
	//拿到资产句柄
	HAPI_StringHandle assetSh;
	HAPI_GetAvailableAssets(&houSession, (HAPI_AssetLibraryId)inlibrary_id, &assetSh, assetsCount);
	fullOptionName = ToString(inhoudiniSession, assetSh);
	TArray<FString> outStringArray;
	fullOptionName.ParseIntoArray(outStringArray, TEXT("/"), true);
	label = outStringArray[outStringArray.Num() - 1];
	return false;
}

FString UHoudiniEngineBPLibrary::ToString(FHoudiniSession inhoudiniSession, HAPI_StringHandle inAssethandle)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return FString("");
	check(inAssethandle >= 0);
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	int32 stringLength = 0;
	HAPI_Result tempResult = HAPI_GetStringBufLength(nullptr, inAssethandle, &stringLength);
	check(stringLength > 0);
	TArray<ANSICHAR> stringBuffer;
	stringBuffer.SetNumUninitialized(stringLength);
	stringBuffer.Last() = 0;
	tempResult =HAPI_GetString(nullptr, inAssethandle, stringBuffer.GetData(), stringLength);
	check(tempResult == HAPI_RESULT_SUCCESS);
	FString finalStr = UTF8_TO_TCHAR(stringBuffer.GetData());
	return finalStr;
}
