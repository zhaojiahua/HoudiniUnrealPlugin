
#include "HoudiniEngineBPLibrary.h"
#include "HoudiniEngine.h"
#include "HoudiniEngineUtilityLibrary.h"

FHoudiniSession UHoudiniEngineBPLibrary::globalHSession;

UHoudiniEngineBPLibrary::UHoudiniEngineBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UHoudiniEngineBPLibrary::HoudiniEngineSampleFunction(float Param)
{
	return -1;
}

FHoudiniSession& UHoudiniEngineBPLibrary::GetGlobalHoudiniSession()
{
	return globalHSession;
}

bool UHoudiniEngineBPLibrary::GetValidGlobalHoudiniSession(FHoudiniSession& outHoudiniSession)
{
	if (HoudiniSessionIsValid(globalHSession))
	{
		outHoudiniSession = globalHSession;
		return true;
	}
	if (StartServerAndCreateSession())
	{
		outHoudiniSession = globalHSession;
		return true;
	}
	return false;
}

bool UHoudiniEngineBPLibrary::StartServerAndCreateSession()
{
	HAPI_Session  houdiniSession = globalHSession.ToHAPI_Session();
	HAPI_Result createResult = HAPI_CreateThriftNamedPipeSession(&houdiniSession, "hapi");
	if (createResult != HAPI_Result::HAPI_RESULT_SUCCESS)
	{
		HAPI_ThriftServerOptions serverOptions{ 0 };
		FMemory::Memzero<HAPI_ThriftServerOptions>(serverOptions);
		serverOptions.autoClose = true;
		serverOptions.timeoutMs = 3000.0f;
		HAPI_Result tempResult = HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr);
		if (tempResult == HAPI_Result::HAPI_RESULT_SUCCESS)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Houdini named pipe server has been successfully started!"));
			UHoudiniEngineUtilityLibrary::HoudiniNotification(TEXT("Houdini named pipe server has been successfully started!"));
		}
		else
		{
			UHoudiniEngineUtilityLibrary::HoudiniNotification(TEXT("Houdini named pipe server started failed! "));
		}
		tempResult = HAPI_CreateThriftNamedPipeSession(&houdiniSession, "hapi");
		if (tempResult == HAPI_Result::HAPI_RESULT_SUCCESS)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Houdini named pipe server has been successfully created!"));
			UHoudiniEngineUtilityLibrary::HoudiniNotification(TEXT("Houdini named pipe server has been successfully created!"));
			globalHSession = FHoudiniSession::FromHAPI_Session(houdiniSession);
			return true;
		}
		else
		{
			UHoudiniEngineUtilityLibrary::HoudiniNotification(TEXT("Houdini named pipe server created failed"));
			return false;
		}
	}
	UHoudiniEngineUtilityLibrary::HoudiniNotification(TEXT("Houdini named pipe server has been successfully created!"));
	globalHSession = FHoudiniSession::FromHAPI_Session(houdiniSession);
	return true;
}

FHoudiniCookOption UHoudiniEngineBPLibrary::CreateHoudiniCookOption()
{
	HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
	FHoudiniCookOption tempHoudiniCookOption;
	tempHoudiniCookOption.houdiniCooOption = cookOptions;
	return tempHoudiniCookOption;
}

void UHoudiniEngineBPLibrary::HoudiniInitializeSession(FHoudiniSession inhoudiniSession,const FHoudiniCookOption& inhoudiniCookOption)
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

bool UHoudiniEngineBPLibrary::HoudiniGetAssetInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniAssetInfo& outAssetInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetAssetInfo(&houSession, (HAPI_NodeId)inNodeId, &outAssetInfo.houAssetInfo);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::GetAssetInfoSubData(FHoudiniSession inhoudiniSession,const FHoudiniAssetInfo& outAssetInfo, bool& bHasCooked, FString& outNodeName, FString& outNodeLabel, FString& outFullOpName)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	bHasCooked = (bool)outAssetInfo.houAssetInfo.hasEverCooked;
	outNodeName = ToString(inhoudiniSession, outAssetInfo.houAssetInfo.nameSH);
	outNodeLabel = ToString(inhoudiniSession, outAssetInfo.houAssetInfo.labelSH);
	outFullOpName = ToString(inhoudiniSession, outAssetInfo.houAssetInfo.fullOpNameSH);
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniCreateNode(FHoudiniSession inhoudiniSession, FString optionName, FString label, int& nodeId, int parentNodeId, bool bcookOnCreation)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_NodeId houNodeId;
	HAPI_Result tempResult = HAPI_CreateNode(&houSession, (HAPI_NodeId)parentNodeId, TCHAR_TO_UTF8(*optionName), TCHAR_TO_UTF8(*label), (HAPI_Bool)bcookOnCreation, &houNodeId);
	nodeId = (int)houNodeId;
	return tempResult == HAPI_RESULT_SUCCESS;
}

void UHoudiniEngineBPLibrary::HoudiniCookNode(FHoudiniSession inhoudiniSession, int nodeid, const FHoudiniCookOption& inHoudiniCookOption)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_CookNode(&houSession, (HAPI_NodeId)nodeid, &inHoudiniCookOption.houdiniCooOption);
}

bool UHoudiniEngineBPLibrary::HoudiniSaveHipFile(FHoudiniSession inhoudiniSession, FString inFilePath, bool blookNodes)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SaveHIPFile(&houSession, TCHAR_TO_UTF8(*inFilePath), (HAPI_Bool)blookNodes);
	return tempResult == HAPI_RESULT_SUCCESS;
}

void UHoudiniEngineBPLibrary::HoudiniClearUpSession(FHoudiniSession inhoudiniSession)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Cleanup(&houSession);
}

void UHoudiniEngineBPLibrary::HoudiniCloseSession(FHoudiniSession inhoudiniSession)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_CloseSession(&houSession);
}

bool UHoudiniEngineBPLibrary::HoudiniSessionIsInitialized(FHoudiniSession inhoudiniSession)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_IsInitialized(&houSession);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniComposeChildNodeList(FHoudiniSession inhoudiniSession, int inParentNodeId, EHoudini_NodeType inNodeType, EHoudini_NodeFlag inNodeFlag, int& childCount, bool brecursive)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	FEnumParser<HAPI_NodeType> nodeTypeEnumParsar;
	FEnumParser<HAPI_NodeFlags> nodeFlagsEnumParsar;
	HAPI_Result tempResult = HAPI_ComposeChildNodeList(&houSession, (HAPI_NodeId)inParentNodeId, (HAPI_NodeTypeBits)nodeTypeEnumParsar.ParsarEnum(HoudiniEnumToString(inNodeType)), (HAPI_NodeFlagsBits)nodeFlagsEnumParsar.ParsarEnum(HoudiniEnumToString(inNodeFlag)), brecursive, &childCount);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetComposedChildNodeList(FHoudiniSession inhoudiniSession, int inParentNodeId, TArray<int>& childNodeList, int childCount)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	childNodeList.SetNumUninitialized(childCount);
	HAPI_Result tempResult = HAPI_GetComposedChildNodeList(&houSession, (HAPI_NodeId)inParentNodeId, (HAPI_NodeId*)childNodeList.GetData(), childCount);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniConnectNodeInput(FHoudiniSession inhoudiniSession, int inNextNodeId, int inputIndex, int inCurrentNodeId, int outIndex)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_ConnectNodeInput(&houSession, (HAPI_NodeId)inNextNodeId, inputIndex, (HAPI_NodeId)inCurrentNodeId, outIndex);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniQueryNodeInput(FHoudiniSession inhoudiniSession, int nodeToQueryId, int inputIndex, int& connectedNodeId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_NodeId tempConnectedNodeId;
	HAPI_Result tempResult = HAPI_QueryNodeInput(&houSession, (HAPI_NodeId)nodeToQueryId, inputIndex, &tempConnectedNodeId);
	connectedNodeId = tempConnectedNodeId;
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniDisconnectNodeInput(FHoudiniSession inhoudiniSession, int nodeId, int inputIndex)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_DisconnectNodeInput(&houSession, (HAPI_NodeId)nodeId, inputIndex);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniDeleteNode(FHoudiniSession inhoudiniSession, int nodeId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_DeleteNode(&houSession, (HAPI_NodeId)nodeId);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniIsNodeValid(FHoudiniSession inhoudiniSession, int nodeId)
{
	if (nodeId < 0) return false;
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_NodeInfo tempNodeInfo;
	HAPI_Result tempResult = HAPI_GetNodeInfo(&houSession, (HAPI_NodeId)nodeId, &tempNodeInfo);
	if (tempResult != HAPI_RESULT_SUCCESS) return false;
	bool tempAnswer;
	HAPI_IsNodeValid(&houSession, (HAPI_NodeId)nodeId, tempNodeInfo.uniqueHoudiniNodeId, &(HAPI_Bool)tempAnswer);
	return tempAnswer;
}

bool UHoudiniEngineBPLibrary::HoudiniCreateInputNode(FHoudiniSession inhoudiniSession, int& nodeId, FString nameLabel)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_NodeId tempNodeId;
	HAPI_Result tempResult = HAPI_CreateInputNode(&houSession, &tempNodeId, TCHAR_TO_UTF8(*nameLabel));
	nodeId = (int)tempNodeId;
	return tempResult == HAPI_RESULT_SUCCESS;
}

FHoudiniPartInfo UHoudiniEngineBPLibrary::HoudiniCreatePartInfo(int infaceCount, int invertexCount, int inpointsCount, EHoudini_PartType houPartType)
{
	HAPI_PartInfo newNodePart = HAPI_PartInfo_Create();
	newNodePart.faceCount = infaceCount;
	newNodePart.vertexCount = invertexCount;
	newNodePart.pointCount = inpointsCount;
	FEnumParser<HAPI_PartType>FEnumParser;
	newNodePart.type = FEnumParser.ParsarEnum(HoudiniEnumToString(houPartType));
	FHoudiniPartInfo tempHoudiniPartInfo;
	tempHoudiniPartInfo.houPartInfo = newNodePart;
	return tempHoudiniPartInfo;
}

bool UHoudiniEngineBPLibrary::HoudiniSetPartInfo(FHoudiniSession inhoudiniSession, int nodeId, const FHoudiniPartInfo& inHouPartInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetPartInfo(&houSession, (HAPI_NodeId)nodeId, 0, &inHouPartInfo.houPartInfo);
	return tempResult == HAPI_RESULT_SUCCESS;
}

FHoudiniAtrributeInfo UHoudiniEngineBPLibrary::HoudiniCreateAttributeInfo(int incount, int intupleSize/*=1*/, bool bexists /*= true*/, EHoudini_AttributeOwner attrOwner /*=EHoudini_AttributeOwner::Houdini_ATTROWNER_POINT*/, EHoudini_StorageType stoType  /*= EHoudini_StorageType::Houdini_STORAGETYPE_FLOAT*/)
{
	FHoudiniAtrributeInfo tempoutAttrInfo;
	HAPI_AttributeInfo  houAttributeInfo = HAPI_AttributeInfo_Create();
	houAttributeInfo.count = incount;
	houAttributeInfo.tupleSize = intupleSize;
	houAttributeInfo.exists = bexists;
	FEnumParser<HAPI_AttributeOwner>AttributeOwnerEnumParser;
	FEnumParser<HAPI_StorageType>storageTypeEnumParser;
	houAttributeInfo.owner = AttributeOwnerEnumParser.ParsarEnum(HoudiniEnumToString(attrOwner));
	houAttributeInfo.storage = storageTypeEnumParser.ParsarEnum(HoudiniEnumToString(stoType));
	tempoutAttrInfo.houAttributeInfo = houAttributeInfo;
	return tempoutAttrInfo;
}

bool UHoudiniEngineBPLibrary::HoudiniAddAndSetFloatAttr(FHoudiniSession inhoudiniSession, int inNodeId, FString attrName, const FHoudiniAtrributeInfo& inAttrInfo, const TArray<float>& attrDatas, int incount)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult1 = HAPI_AddAttribute(&houSession, (HAPI_NodeId)inNodeId, 0, TCHAR_TO_UTF8(*attrName), &inAttrInfo.houAttributeInfo);
	HAPI_Result tempResult2 = HAPI_SetAttributeFloatData(&houSession, (HAPI_NodeId)inNodeId, 0, TCHAR_TO_UTF8(*attrName), &inAttrInfo.houAttributeInfo, attrDatas.GetData(), 0, incount);
	return tempResult1 == HAPI_RESULT_SUCCESS && tempResult2 == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniAddAndSetIntAttr(FHoudiniSession inhoudiniSession, int inNodeId, FString attrName, const FHoudiniAtrributeInfo& inAttrInfo, const TArray<int>& attrDatas, int incount)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult1 = HAPI_AddAttribute(&houSession, (HAPI_NodeId)inNodeId, 0, TCHAR_TO_UTF8(*attrName), &inAttrInfo.houAttributeInfo);
	HAPI_Result tempResult2 = HAPI_SetAttributeIntData(&houSession, (HAPI_NodeId)inNodeId, 0, TCHAR_TO_UTF8(*attrName), &inAttrInfo.houAttributeInfo, attrDatas.GetData(), 0, incount);
	return tempResult1 == HAPI_RESULT_SUCCESS && tempResult2 == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetVertexListAndFaceCount(FHoudiniSession inhoudiniSession, int inNodeId, const TArray<int>& vertexDataArray)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult1 = HAPI_SetVertexList(&houSession, (HAPI_NodeId)inNodeId, 0, vertexDataArray.GetData(), 0, vertexDataArray.Num());
	TArray<int>faceArray;
	faceArray.Init(3, vertexDataArray.Num() / 3);
	HAPI_Result tempResult2 = HAPI_SetFaceCounts(&houSession, (HAPI_NodeId)inNodeId, 0, faceArray.GetData(), 0, faceArray.Num());
	return tempResult1 == HAPI_RESULT_SUCCESS && tempResult2 == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniCommitGeo(FHoudiniSession inhoudiniSession, int inNodeId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_CommitGeo(&houSession, (HAPI_NodeId)inNodeId);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetAtrrInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo& outAttrInfo, EHoudini_AttributeOwner attrOwner)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	FEnumParser<HAPI_AttributeOwner>AttributeOwnerEnumParser;
	HAPI_Result tempResult = HAPI_GetAttributeInfo(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, TCHAR_TO_UTF8(*inAtrrName), AttributeOwnerEnumParser.ParsarEnum(HoudiniEnumToString(attrOwner)), &outAttrInfo.houAttributeInfo);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetAttriIntArrayData(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo outAttrInfo, TArray<int>& outDataArray)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	outDataArray.SetNumUninitialized(outAttrInfo.houAttributeInfo.count * outAttrInfo.houAttributeInfo.tupleSize);
	HAPI_Result tempResult = HAPI_GetAttributeIntData(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, TCHAR_TO_UTF8(*inAtrrName), &outAttrInfo.houAttributeInfo, -1, outDataArray.GetData(), 0, outAttrInfo.houAttributeInfo.count);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetAttriFloatArrayData(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo outAttrInfo, TArray<float>& outDataArray)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	outDataArray.SetNumUninitialized(outAttrInfo.houAttributeInfo.count * outAttrInfo.houAttributeInfo.tupleSize);
	HAPI_Result tempResult = HAPI_GetAttributeFloatData(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, TCHAR_TO_UTF8(*inAtrrName), &outAttrInfo.houAttributeInfo, -1, outDataArray.GetData(), 0, outAttrInfo.houAttributeInfo.count);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetAttriStringArrayData(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo outAttrInfo, TArray<FString>& outDataArray)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	TArray<HAPI_StringHandle> stringHandleList;
	int stringCount = outAttrInfo.houAttributeInfo.count * outAttrInfo.houAttributeInfo.tupleSize;
	stringHandleList.SetNumUninitialized(stringCount);
	HAPI_Result tempResult = HAPI_GetAttributeStringData(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, TCHAR_TO_UTF8(*inAtrrName), &outAttrInfo.houAttributeInfo, stringHandleList.GetData(), 0, stringCount);
	if (tempResult != HAPI_RESULT_SUCCESS)return false;
	TMap<HAPI_StringHandle, FString> houStringToFStringMap;
	outDataArray.SetNum(stringCount);
	for (int i = 0; i < stringCount; i++)
	{
		if (i > 0 && stringHandleList[i] == stringHandleList[i - 1])
		{
			outDataArray[i] = outDataArray[i - 1];
		}
		const FString* foundStr = houStringToFStringMap.Find(stringHandleList[i]);
		if (foundStr)
		{
			outDataArray[i] = *foundStr;
		}
		else
		{
			outDataArray[i] = ToString(inhoudiniSession, stringHandleList[i]);
			houStringToFStringMap.Add(stringHandleList[i], outDataArray[i]);
		}
	}
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetVertexList(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, TArray<int>& outVertexList, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	outVertexList.SetNumUninitialized(count);
	HAPI_Result tempResult = HAPI_GetVertexList(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, outVertexList.GetData(), 0, count);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetPartInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FHoudiniPartInfo& outPartInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetPartInfo(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, &outPartInfo.houPartInfo);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetPartInfoSubData(FHoudiniSession inhoudiniSession, const FHoudiniPartInfo& intPartInfo, int& faceCount, int& pointCount, EHoudini_PartType& partType, FString& partName, bool& bInstanced, int& instanceCount)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	faceCount = intPartInfo.houPartInfo.faceCount;
	pointCount = intPartInfo.houPartInfo.pointCount;
	FEnumParser<HAPI_PartType> PartTypeParser;
	partType = HoudiniStringToEnum<EHoudini_PartType>(PartTypeParser.FindKey(intPartInfo.houPartInfo.type));
	partName = ToString(inhoudiniSession, intPartInfo.houPartInfo.nameSH);
	bInstanced = intPartInfo.houPartInfo.isInstanced;
	instanceCount = intPartInfo.houPartInfo.instanceCount;
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetNodeInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniNodeInfo& outNodeInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_NodeInfo  tempNodeInfo;
	HAPI_Result tempResult = HAPI_GetNodeInfo(&houSession, (HAPI_NodeId)inNodeId, &tempNodeInfo);
	outNodeInfo.houNodeInfo = tempNodeInfo;
	return tempResult == HAPI_RESULT_SUCCESS;
}

void UHoudiniEngineBPLibrary::HoudiniGetNodeInfoSubData(const FHoudiniNodeInfo& outNodeInfo, int& outParentId, int& outUniqueHoudiniNodeId, bool& isValid)
{
	isValid = outNodeInfo.houNodeInfo.isValid;
	outParentId=outNodeInfo.houNodeInfo.parentId;
	outUniqueHoudiniNodeId = outNodeInfo.houNodeInfo.uniqueHoudiniNodeId;
}

bool UHoudiniEngineBPLibrary::HoudiniSetObjectTransform(FHoudiniSession inhoudiniSession, int inNodeId, const FTransform& inTransform)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_TransformEuler& houTransformEuler =UHoudiniEngineUtilityLibrary::UnrealTransformToHoudiniTransformEuler(inTransform).houTransformEuler;
	HAPI_Result tempResult = HAPI_SetObjectTransform(&houSession, (HAPI_NodeId)inNodeId, &houTransformEuler);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetObjectTransform(FHoudiniSession inhoudiniSession, int inNodeId, int relativeNodeId, FTransform& outTransform)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Transform tempHouTransform;
	HAPI_Transform_Init(&tempHouTransform);
	HAPI_Result tempResult = HAPI_GetObjectTransform(&houSession, (HAPI_NodeId)inNodeId, (HAPI_NodeId)relativeNodeId, HAPI_SRT, &tempHouTransform);
	FHoudiniTranform tempOutHoudiniTransform;
	tempOutHoudiniTransform.houTransform = tempHouTransform;
	outTransform = UHoudiniEngineUtilityLibrary::HoudiniTransformToUnrealTransform(tempOutHoudiniTransform);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniComposeObjectList(FHoudiniSession inhoudiniSession, int inNodeId, int& outObjCount)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_ComposeObjectList(&houSession, (HAPI_NodeId)inNodeId, nullptr, &outObjCount);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetComposedObjectList(FHoudiniSession inhoudiniSession, int inNodeId, TArray<FHoudiniObjectInfo>& outObjInfos, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	TArray<HAPI_ObjectInfo> houObjInfos;
	houObjInfos.SetNumUninitialized(count);
	outObjInfos.SetNumUninitialized(count);
	HAPI_Result tempResult = HAPI_GetComposedObjectList(&houSession, (HAPI_NodeId)inNodeId, houObjInfos.GetData(), 0, count);
	if (tempResult != HAPI_RESULT_SUCCESS)return false;
	for (int i = 0; i < houObjInfos.Num(); i++)
	{
		outObjInfos[i].houObjectInfo = houObjInfos[i];
	}
	return  true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetObjectInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniObjectInfo& outObjInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetObjectInfo(&houSession, (HAPI_NodeId)inNodeId, &outObjInfo.houObjectInfo);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetDisplayGeoInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniGeoInfo& outGeoInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetDisplayGeoInfo(&houSession, (HAPI_NodeId)inNodeId, &outGeoInfo.houGeoInfo);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

void UHoudiniEngineBPLibrary::HoudiniGetObjInfoSubData(const FHoudiniObjectInfo& inObjInfo, int& outNodeId, bool& bIsVisible)
{
	outNodeId = (int)inObjInfo.houObjectInfo.nodeId;
	bIsVisible = inObjInfo.houObjectInfo.isVisible;
}

void UHoudiniEngineBPLibrary::HoudiniGetGeoInfoSubData(const FHoudiniGeoInfo& inGeoInfo, int& outSopNodeId, bool& isTemplated, bool& isDisplayGeo, int& outPartCount)
{
	outSopNodeId = inGeoInfo.houGeoInfo.nodeId;
	isTemplated = inGeoInfo.houGeoInfo.isTemplated;
	isDisplayGeo = inGeoInfo.houGeoInfo.isDisplayGeo;
	outPartCount = inGeoInfo.houGeoInfo.partCount;
}

FHoudiniCurveInfo UHoudiniEngineBPLibrary::HoudiniCreateCurveInfo(int crvCount, int pointCount, EHoudini_CurveType crvType, bool bHasKnots, int knotCount, int knotOrder, bool bIsPeriodic)
{
	HAPI_CurveInfo tempCurveInfo = HAPI_CurveInfo_Create();
	tempCurveInfo.curveCount = crvCount;
	tempCurveInfo.vertexCount = pointCount;
	FEnumParser<HAPI_CurveType> CrvTypeParser;
	tempCurveInfo.curveType = CrvTypeParser.ParsarEnum(HoudiniEnumToString(crvType));
	tempCurveInfo.hasKnots = bHasKnots;
	tempCurveInfo.knotCount = knotCount;
	tempCurveInfo.order = knotOrder;
	tempCurveInfo.isPeriodic = bIsPeriodic;
	FHoudiniCurveInfo tempHoudiniCurveInfo;
	tempHoudiniCurveInfo.houCurvetInfo = tempCurveInfo;
	return tempHoudiniCurveInfo;
}

bool UHoudiniEngineBPLibrary::HoudiniSetCurveInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, const FHoudiniCurveInfo& inCrvInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetCurveInfo(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, &inCrvInfo.houCurvetInfo);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetCurveCounts(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, const TArray<int>& curveCountsArray)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	if (curveCountsArray.Num() <= 0) { return false; }
	HAPI_Result tempResult = HAPI_SetCurveCounts(&houSession, (HAPI_NodeId)inNodeId, (HAPI_PartId)inPartId, curveCountsArray.GetData(), 0, curveCountsArray.Num());
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmFloatValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, float param_value)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	FHoudiniParamInfo tempParmInfo;
	if (!HoudiniGetParmInfoFromName(inhoudiniSession, inNodeId, param_name, tempParmInfo)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmFloatValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), param_index, param_value);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmIntValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, int param_value)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	FHoudiniParamInfo tempParmInfo;
	if (!HoudiniGetParmInfoFromName(inhoudiniSession, inNodeId, param_name, tempParmInfo)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmIntValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), param_index, param_value);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmStringValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_value, int param_id, int param_index)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmStringValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_value), (HAPI_ParmId)param_id, param_index);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmNodeValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int valueNodeId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmNodeValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), (HAPI_NodeId)valueNodeId);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmExpression(FHoudiniSession inhoudiniSession, int inNodeId, FString param_value, int param_id, int param_index)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmExpression(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_value), (HAPI_ParmId)param_id, param_index);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmFloatValues(FHoudiniSession inhoudiniSession, int inNodeId, const TArray<float>& inValues, int start)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmFloatValues(&houSession, (HAPI_NodeId)inNodeId, inValues.GetData(), start, inValues.Num());
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniSetParmIntValues(FHoudiniSession inhoudiniSession, int inNodeId, const TArray<int>& inValues, int start)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_SetParmIntValues(&houSession, (HAPI_NodeId)inNodeId, inValues.GetData(), start, inValues.Num());
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniRevertParmToDefault(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_RevertParmToDefault(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), param_index);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniRevertParmToDefaults(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_RevertParmToDefaults(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name));
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmFloatValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, float& outparam_value)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetParmFloatValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), param_index, &outparam_value);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmIntValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, int& outparam_value)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetParmIntValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), param_index, &outparam_value);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmStringValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, bool bEvalueate, FString& outStrValue)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_StringHandle tempSH = -1;
	HAPI_Result tempResult = HAPI_GetParmStringValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), param_index, bEvalueate, &tempSH);
	if (tempSH < 0 || tempResult != HAPI_RESULT_SUCCESS) return false;
	outStrValue = ToString(inhoudiniSession, tempSH);
	return  true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmNodeValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int& outNodeId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_NodeId tempNodeId = -1;
	HAPI_Result tempResult = HAPI_GetParmNodeValue(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), &tempNodeId);
	if (tempNodeId < 0 || tempResult != HAPI_RESULT_SUCCESS) return false;
	outNodeId = (int)tempNodeId;
	return  true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmFloatValues(FHoudiniSession inhoudiniSession, int inNodeId, TArray<float>& outValues, int startIndex, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	outValues.SetNumUninitialized(count);
	HAPI_Result tempResult = HAPI_GetParmFloatValues(&houSession, (HAPI_NodeId)inNodeId, outValues.GetData(), startIndex, count);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmIntValues(FHoudiniSession inhoudiniSession, int inNodeId, TArray<int>& outValues, int startIndex, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	outValues.SetNumUninitialized(count);
	HAPI_Result tempResult = HAPI_GetParmIntValues(&houSession, (HAPI_NodeId)inNodeId, outValues.GetData(), startIndex, count);
	return  tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmStringValues(FHoudiniSession inhoudiniSession, int inNodeId, bool bEvalueate, TArray<FString>& outStrValues,int startIndex, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	TArray<HAPI_StringHandle> strHandles;
	strHandles.SetNumUninitialized(count);
	//outStrValues.SetNumUninitialized(count);
	HAPI_Result tempResult = HAPI_GetParmStringValues(&houSession, (HAPI_NodeId)inNodeId, bEvalueate, strHandles.GetData(), startIndex, count);
	if (tempResult != HAPI_RESULT_SUCCESS)return false;
	TMap<HAPI_StringHandle, FString> handleToStringMap;
	for (int i = 0; i < count; i++)
	{
		if (i > 0 && strHandles[i] == strHandles[i - 1])
		{
			strHandles[i] = strHandles[i - 1];
		}
		const FString* tempFindStr = handleToStringMap.Find(strHandles[i]);
		if (tempFindStr)
		{
			outStrValues.Add(*tempFindStr);
		}
		else
		{
			outStrValues.Add(ToString(inhoudiniSession, strHandles[i]));
			handleToStringMap.Add(strHandles[i], outStrValues[i]);
		}
		//outStrValues.Add(ToString(inhoudiniSession, strHandles[i]));
	}
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParameters(FHoudiniSession inhoudiniSession, int inNodeId, TArray<FHoudiniParamInfo>& parmInfoList, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession) || count <= 0) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	TArray< HAPI_ParmInfo > houParmInfos;
	houParmInfos.SetNumUninitialized(count);
	HAPI_Result tempResult = HAPI_GetParameters(&houSession, (HAPI_NodeId)inNodeId, houParmInfos.GetData(), 0, count);
	if (tempResult != HAPI_RESULT_SUCCESS) return false;
	for (int i = 0; i < count; i++)
	{
		FHoudiniParamInfo tempParmInfo;
		tempParmInfo.houParmInfo = houParmInfos[i];
		parmInfoList.Add(tempParmInfo);
	}
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniParmInfo_IsInt(const FHoudiniParamInfo& inParmInfo)
{
	return HAPI_ParmInfo_IsInt(&inParmInfo.houParmInfo);
}

bool UHoudiniEngineBPLibrary::HoudiniParmInfo_IsFloat(const FHoudiniParamInfo& inParmInfo)
{
	return HAPI_ParmInfo_IsFloat(&inParmInfo.houParmInfo);
}

bool UHoudiniEngineBPLibrary::HoudiniParmInfo_IsString(const FHoudiniParamInfo& inParmInfo)
{
	return HAPI_ParmInfo_IsString(&inParmInfo.houParmInfo);
}

int UHoudiniEngineBPLibrary::HoudiniGetParmInfoTypeValueCount(const FHoudiniParamInfo& inParmInfo, EHoudini_ParamValueType invalueType)
{
	int tempCount = 0;
	switch (invalueType)
	{
	case EHoudini_ParamValueType::Houdini_ParamValue_float:
		tempCount = HAPI_ParmInfo_GetFloatValueCount(&inParmInfo.houParmInfo);
		break;
	case EHoudini_ParamValueType::Houdini_ParamValue_int:
		tempCount = HAPI_ParmInfo_GetIntValueCount(&inParmInfo.houParmInfo);
		break;
	case EHoudini_ParamValueType::Houdini_ParamValue_string:
		tempCount = HAPI_ParmInfo_GetStringValueCount(&inParmInfo.houParmInfo);
		break;
	default:
		break;
	}
	return tempCount;
}

int UHoudiniEngineBPLibrary::HoudiniGetParmInfoValueIndex(const FHoudiniParamInfo& inParmInfo, EHoudini_ParamValueIndexType invalueIndexType)
{
	int tempindex = -1;
	switch (invalueIndexType)
	{
	case EHoudini_ParamValueIndexType::Houdini_ParamValueIndex_float:
		tempindex = inParmInfo.houParmInfo.floatValuesIndex;
		break;
	case EHoudini_ParamValueIndexType::Houdini_ParamValueIndex_int:
		tempindex = inParmInfo.houParmInfo.intValuesIndex;
		break;
	case EHoudini_ParamValueIndexType::Houdini_ParamValueIndex_string:
		tempindex = inParmInfo.houParmInfo.stringValuesIndex;
		break;
	case EHoudini_ParamValueIndexType::Houdini_ParamValueIndex_choice:
		tempindex = inParmInfo.houParmInfo.choiceIndex;
		break;
	default:
		break;
	}
	return tempindex;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmInfoFromName(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, FHoudiniParamInfo& outParamInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetParmInfoFromName(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), &outParamInfo.houParmInfo);
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmIdFromName(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int& outParamId)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_ParmId tempParmId;
	HAPI_Result tempResult = HAPI_GetParmIdFromName(&houSession, (HAPI_NodeId)inNodeId, TCHAR_TO_UTF8(*param_name), &tempParmId);
	outParamId = (int)tempParmId;
	return tempResult == HAPI_RESULT_SUCCESS;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inParmId, FHoudiniParamInfo& outParamInfo)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_Result tempResult = HAPI_GetParmInfo(&houSession, (HAPI_NodeId)inNodeId, (HAPI_ParmId)inParmId, &outParamInfo.houParmInfo);
	return tempResult == HAPI_RESULT_SUCCESS;
}

int UHoudiniEngineBPLibrary::HoudiniGetChoiceCount(const FHoudiniParamInfo& inParmInfo)
{
	return inParmInfo.houParmInfo.choiceCount;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmChoiceLists(FHoudiniSession inhoudiniSession, int inNodeId, TArray<FHoudiniParmChoiceInfo>& outParmChoiceInfos, int start, int count)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	TArray<  HAPI_ParmChoiceInfo > temHAPI_ParmChoiceInfos;
	HAPI_Result tempResult = HAPI_GetParmChoiceLists(&houSession, (HAPI_NodeId)inNodeId, temHAPI_ParmChoiceInfos.GetData(), start, count);
	if (tempResult != HAPI_RESULT_SUCCESS) return false;
	for (auto item : temHAPI_ParmChoiceInfos)
	{
		FHoudiniParmChoiceInfo tempParmChoiceInfo;
		tempParmChoiceInfo.houParmChoiceInfo = item;
		outParmChoiceInfos.Add(tempParmChoiceInfo);
	}
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniGetParmChoiceInfoSubData(FHoudiniSession inhoudiniSession, const TArray<FHoudiniParmChoiceInfo>& inChoiceInfo, TArray<FString>& outValues, TArray<FString>& outLabels)
{
	if (inChoiceInfo.Num() <= 0 || !HoudiniSessionIsValid(inhoudiniSession)) return false;
	for (auto item : inChoiceInfo)
	{
		outValues.Add(ToString(inhoudiniSession, item.houParmChoiceInfo.valueSH));
		outLabels.Add(ToString(inhoudiniSession, item.houParmChoiceInfo.labelSH));
	}
	return true;
}

bool UHoudiniEngineBPLibrary::HoudiniParmInfoIsNonValue(const FHoudiniParamInfo& inParmInfo)
{
	return HAPI_ParmInfo_IsNonValue(&inParmInfo.houParmInfo);
}

void UHoudiniEngineBPLibrary::HoudiniGetMultiParmSubData(const FHoudiniParamInfo& inParmInfo, int& outinstanceNum, int& outinstanceCount, int& outinstanceLength, int& outinstanceStartOffset, EHoudini_RampType& outRampType)
{
	outinstanceNum = inParmInfo.houParmInfo.instanceNum;
	outinstanceCount = inParmInfo.houParmInfo.instanceCount;
	outinstanceLength = inParmInfo.houParmInfo.instanceLength;
	outinstanceStartOffset = inParmInfo.houParmInfo.instanceStartOffset;
	FEnumParser<HAPI_RampType> rampTypeParse;
	outRampType = HoudiniStringToEnum<EHoudini_RampType>(rampTypeParse.FindKey(inParmInfo.houParmInfo.rampType));
}


FString UHoudiniEngineBPLibrary::ToString(FHoudiniSession inhoudiniSession, HAPI_StringHandle inAssethandle)
{
	if (!HoudiniSessionIsValid(inhoudiniSession)) return FString("");
	check(inAssethandle >= 0);
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	int32 stringLength = 0;
	HAPI_Result tempResult = HAPI_GetStringBufLength(&houSession, inAssethandle, &stringLength);
	check(tempResult == HAPI_RESULT_SUCCESS);
	check(stringLength > 0);
	TArray<ANSICHAR> stringBuffer;
	stringBuffer.SetNumUninitialized(stringLength);
	stringBuffer.Last() = 0;
	tempResult = HAPI_GetString(&houSession, inAssethandle, stringBuffer.GetData(), stringLength);
	check(tempResult == HAPI_RESULT_SUCCESS);
	FString finalStr = UTF8_TO_TCHAR(stringBuffer.GetData());
	return finalStr;
}


