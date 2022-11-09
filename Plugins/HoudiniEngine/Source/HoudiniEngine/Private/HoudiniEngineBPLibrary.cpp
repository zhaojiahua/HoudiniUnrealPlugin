
#include "HoudiniEngineBPLibrary.h"
#include "HoudiniEngine.h"
#include "HoudiniEngineUtilityLibrary.h"

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

void UHoudiniEngineBPLibrary::HoudiniGetGeoFromPartInfo(const FHoudiniPartInfo& intPartInfo, int& faceCount, int& pointCount)
{
	faceCount = intPartInfo.houPartInfo.faceCount;
	pointCount = intPartInfo.houPartInfo.pointCount;
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
	outNodeId = inObjInfo.houObjectInfo.nodeId;
	bIsVisible = inObjInfo.houObjectInfo.isVisible;
}

void UHoudiniEngineBPLibrary::HoudiniGetGeoInfoSubData(const FHoudiniGeoInfo& inGeoInfo, int& outSopNodeId, bool& isTemplated, bool& isDisplayGeo, int& outPartCount)
{
	outSopNodeId = inGeoInfo.houGeoInfo.nodeId;
	isTemplated = inGeoInfo.houGeoInfo.isTemplated;
	isDisplayGeo = inGeoInfo.houGeoInfo.isDisplayGeo;
	outPartCount = inGeoInfo.houGeoInfo.partCount;
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


