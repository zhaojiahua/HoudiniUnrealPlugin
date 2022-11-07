// Copyright Epic Games, Inc. All Rights Reserved.

#include "HoudiniEngine.h"

#define LOCTEXT_NAMESPACE "FHoudiniEngineModule"

void FHoudiniEngineModule::StartupModule()
{
	//在插件开始加载的时候设置环境变量
	FString HAPIDLLLocation = LR"(C:\Program Files\Side Effects Software\Houdini 18.0.287\bin)";//houdini动态库链接的位置
	const TCHAR* pathDelimiter = FPlatformMisc::GetPathVarDelimiter();//获取当前平台的路径界定符
	FString enviPathValue = FPlatformMisc::GetEnvironmentVariable(TEXT("Path"));//获取当前平台的环境变路径
	FPlatformMisc::SetEnvironmentVar(TEXT("Path"), *(HAPIDLLLocation + pathDelimiter + enviPathValue));//设置当前平台的环境变量

}

void FHoudiniEngineModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHoudiniEngineModule, HoudiniEngine)