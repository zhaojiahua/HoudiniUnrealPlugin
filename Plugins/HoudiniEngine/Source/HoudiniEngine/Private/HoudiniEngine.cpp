// Copyright Epic Games, Inc. All Rights Reserved.

#include "HoudiniEngine.h"

#define LOCTEXT_NAMESPACE "FHoudiniEngineModule"

void FHoudiniEngineModule::StartupModule()
{
	//�ڲ����ʼ���ص�ʱ�����û�������
	FString HAPIDLLLocation = LR"(C:\Program Files\Side Effects Software\Houdini 18.0.287\bin)";//houdini��̬�����ӵ�λ��
	const TCHAR* pathDelimiter = FPlatformMisc::GetPathVarDelimiter();//��ȡ��ǰƽ̨��·���綨��
	FString enviPathValue = FPlatformMisc::GetEnvironmentVariable(TEXT("Path"));//��ȡ��ǰƽ̨�Ļ�����·��
	FPlatformMisc::SetEnvironmentVar(TEXT("Path"), *(HAPIDLLLocation + pathDelimiter + enviPathValue));//���õ�ǰƽ̨�Ļ�������

}

void FHoudiniEngineModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHoudiniEngineModule, HoudiniEngine)