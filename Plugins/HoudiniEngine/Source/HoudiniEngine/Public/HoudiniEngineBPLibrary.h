// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HAPIToUnrealCommonBridge.h"
#include "HoudiniEngineBPLibrary.generated.h"


UCLASS()
class UHoudiniEngineBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "HoudiniEngine sample test testing"), Category = "HoudiniEngineTesting")
	static float HoudiniEngineSampleFunction(float Param);

public:
		static FHoudiniSession& GetGlobalHoudiniSession();
// get global houdini session
	UFUNCTION(BlueprintCallable,BlueprintPure, category = "zjhHoudiniUnrealPlugin | Sessions")
		static bool GetValidGlobalHoudiniSession(FHoudiniSession& outHoudiniSession);
	//UFUNCTION(BlueprintCallable, meta = (ToolTip = "Creates a Thrift RPC session using a Windows named pipe or a Unix domain socket as transport. "), category = "zjhHoudiniUnrealPlugin | Sessions")
		static bool StartServerAndCreateSession();
		UFUNCTION(BlueprintCallable, meta = (ToolTip = "Creates options which affect how nodes are cooked. "), category = "zjhHoudiniUnrealPlugin")
			static FHoudiniCookOption CreateHoudiniCookOption(EHoudini_PackedPrimInstancingMode instancingMode = EHoudini_PackedPrimInstancingMode::Houdini_PACKEDPRIM_INSTANCING_MODE_FLAT);
//		Create the asset manager, set up environment variables, and initialize the main Houdini scene. No license check is done during this step. Only when you try to load an asset library (OTL) do we actually check for licenses. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Sessions")
		static void HoudiniInitializeSession(FHoudiniSession inhoudiniSession, const FHoudiniCookOption& inhoudiniCookOption);
//		CChecks whether the session identified by HAPI_Session::id is a valid session opened in the implementation identified by HAPI_Session::type. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Sessions")
		static bool HoudiniSessionIsValid(FHoudiniSession inhoudiniSession);
//		Loads a Houdini asset library (OTL) from a .otl file. It does NOT create anything inside the Houdini scene. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Assets")
		static bool HoudiniLoadAssetLibraryFromFile(FHoudiniSession inhoudiniSession, FString otlFilePath, bool allowOverWrite, int& assetLibraryId);
//		Get the names of the assets contained in an asset library. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Assets")
		static bool GetAvailableAssetOptionName(FHoudiniSession inhoudiniSession, int inlibrary_id, FString& fullOptionName, FString& label);
//Fill an asset_info struct from a node. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Assets")
		static bool HoudiniGetAssetInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniAssetInfo& outAssetInfo);
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Assets")
		static bool GetAssetInfoSubData(FHoudiniSession inhoudiniSession,const FHoudiniAssetInfo& outAssetInfo, bool& bHasCooked, FString& outNodeName, FString& outNodeLabel, FString& outFullOpName);
//		Create a node inside a node network. Nodes created this way will have their HAPI_NodeInfo::createdPostAssetLoad set to true. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniCreateNode(FHoudiniSession inhoudiniSession, FString optionName, FString label, int& nodeId, int parentNodeId/*= -1*/, bool bcookOnCreation/*=false*/);
//		Initiate a cook on this node. Note that this may trigger cooks on other nodes if they are connected. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static void HoudiniCookNode(FHoudiniSession inhoudiniSession, int nodeid, const FHoudiniCookOption& inHoudiniCookOption);
//		Saves a .hip file of the current Houdini scene.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | HFiles")
		static bool HoudiniSaveHipFile(FHoudiniSession inhoudiniSession, FString inFilePath, bool blookNodes = false);
//		Clean up memory. This will unload all assets and you will need to call HAPI_Initialize() again to be able to use any HAPI methods again. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Sessions")
		static void HoudiniClearUpSession(FHoudiniSession inhoudiniSession);
//		Closes a session. If the session has been established using RPC, then the RPC connection is closed. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Sessions")
		static void HoudiniCloseSession(FHoudiniSession inhoudiniSession);
//		Check whether the runtime has been initialized yet using HAPI_Initialize().
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Sessions")
		static bool HoudiniSessionIsInitialized(FHoudiniSession inhoudiniSession);
//		Compose a list of child nodes based on given filters. This function will only compose the list of child nodes.It will not return this list.After your call to this function, call HAPI_GetComposedChildNodeList() to get the list of child node ids.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniComposeChildNodeList(FHoudiniSession inhoudiniSession, int inParentNodeId, EHoudini_NodeType inNodeType, EHoudini_NodeFlag inNodeFlag, int& childCount, bool brecursive = false);
//		Get the composed list of child node ids from the previous call to HAPI_ComposeChildNodeList(). 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniGetComposedChildNodeList(FHoudiniSession inhoudiniSession, int inParentNodeId, TArray<int>& childNodeList, int childCount);
//		Connect two nodes together. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniConnectNodeInput(FHoudiniSession inhoudiniSession, int inNextNodeId, int inputIndex, int inCurrentNodeId, int outIndex);
//		Query which node is connected to another node's input. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniQueryNodeInput(FHoudiniSession inhoudiniSession, int nodeToQueryId, int inputIndex, int& connectedNodeId);
//Disconnect a node input. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniDisconnectNodeInput(FHoudiniSession inhoudiniSession, int nodeId, int inputIndex);
//Delete a node from a node network. Only nodes with their HAPI_NodeInfo::createdPostAssetLoad set to true can be deleted this way. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniDeleteNode(FHoudiniSession inhoudiniSession, int nodeId);
//Determine if your instance of the node actually still exists inside the Houdini scene. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniIsNodeValid(FHoudiniSession inhoudiniSession, int nodeId);

//		Creates a simple geometry SOP node that can accept geometry input. This will create a dummy OBJ node with a Null SOP inside that you can set the geometry of using the geometry SET APIs.
//		You can then connect this node to any other node as a geometry input. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniCreateInputNode(FHoudiniSession inhoudiniSession, int& nodeId, FString nameLabel);
//Creates a PartInfo struct data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Helper")
		static  FHoudiniPartInfo HoudiniCreatePartInfo(int infaceCount, int invertexCount, int inpointsCount, EHoudini_PartType houPartType = EHoudini_PartType::Houdini_PARTTYPE_MESH);
//Set the main PartInfo struct data
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Helper")
		static bool HoudiniSetPartInfo(FHoudiniSession inhoudiniSession, int nodeId, const FHoudiniPartInfo& inHouPartInfo);

	//Creates a AttributeInfo struct data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Attributes")
		static FHoudiniAtrributeInfo HoudiniCreateAttributeInfo(int incount, int intupleSize = 1, bool bexists = true, EHoudini_AttributeOwner attrOwner = EHoudini_AttributeOwner::Houdini_ATTROWNER_POINT, EHoudini_StorageType stoType = EHoudini_StorageType::Houdini_STORAGETYPE_FLOAT);
//add and set float attribute
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniAddAndSetFloatAttr(FHoudiniSession inhoudiniSession, int inNodeId, FString attrName, const FHoudiniAtrributeInfo& inAttrInfo, const TArray<float>& attrDatas, int incount);
//add and set int attribute
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniAddAndSetIntAttr(FHoudiniSession inhoudiniSession, int inNodeId, FString attrName, const FHoudiniAtrributeInfo& inAttrInfo, const TArray<int>& attrDatas, int incount);
//Set array containing the vertex-point associations where the ith element in the array is the point index the ith vertex associates with. 
//Set the array of faces where the nth integer in the array is the number of vertices the nth face has. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniSetVertexListAndFaceCount(FHoudiniSession inhoudiniSession, int inNodeId, const TArray<int>& vertexDataArray);
//Commit the current input geometry to the cook engine. Nodes that use this geometry node will re-cook using the input geometry given through the geometry setter API calls.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Helper")
		static bool HoudiniCommitGeo(FHoudiniSession inhoudiniSession, int inNodeId);
//Get the attribute info struct for the attribute specified by name. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetAtrrInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo& outAttrInfo, EHoudini_AttributeOwner attrOwner = EHoudini_AttributeOwner::Houdini_ATTROWNER_POINT);
//Get array attribute integer data.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetAttriIntArrayData(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo outAttrInfo, TArray<int>& outDataArray);
//Get array attribute float data.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetAttriFloatArrayData(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo outAttrInfo, TArray<float>& outDataArray);
//Get array attribute string data.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetAttriStringArrayData(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FString inAtrrName, FHoudiniAtrributeInfo outAttrInfo, TArray<FString>& outDataArray);
//Get array containing the vertex-point associations where the ith element in the array is the point index the ith vertex associates with. 
	UFUNCTION(BlueprintCallable,BlueprintPure, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetVertexList(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, TArray<int>& outVertexList, int count);
//Get a particular part info struct. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetPartInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, FHoudiniPartInfo& outPartInfo);
//Get Geo from part info struct
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Attributes")
		static bool HoudiniGetPartInfoSubData(FHoudiniSession inhoudiniSession, const FHoudiniPartInfo& intPartInfo, int& faceCount, int& pointCount, EHoudini_PartType& partType, FString& partName, bool& bInstanced , int& instanceCount);
//Fill an HAPI_NodeInfo struct.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Nodes")
		static bool HoudiniGetNodeInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniNodeInfo& outNodeInfo);
// Get HAPI_NodeInfo struct Sub data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Nodes")
		static void HoudiniGetNodeInfoSubData(const FHoudiniNodeInfo& outNodeInfo, int& outParentId, int& outUniqueHoudiniNodeId, bool& isValid);
//Set the transform of an individual houdini object
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Objects")
		static bool HoudiniSetObjectTransform(FHoudiniSession inhoudiniSession, int inNodeId, const FTransform & inTransform);
//Get the transform of an individual houdini object
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Objects")
		static bool HoudiniGetObjectTransform(FHoudiniSession inhoudiniSession, int inNodeId, int relativeNodeId, FTransform& outTransform);
//Compose a list of child object nodes given a parent node id. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Objects")
		static bool HoudiniComposeObjectList(FHoudiniSession inhoudiniSession, int inNodeId, int& outObjCount);
//Fill an array of HAPI_ObjectInfo structs. 	This is best used with HAPI_ComposeObjectList() with.
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Objects")
		static bool HoudiniGetComposedObjectList(FHoudiniSession inhoudiniSession, int inNodeId, TArray< FHoudiniObjectInfo>& outObjInfos, int count);
//Get the object info on an OBJ node. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Objects")
		static bool HoudiniGetObjectInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniObjectInfo& outObjInfo);
//Get the display geo (SOP) node inside an Object node. If there there are multiple display SOP nodes, only the first one is returned. If the node is a display SOP itself, even if a network, it will return its own geo info. If the node is a SOP but not a network and not the display SOP, this function will fail. 
//The above implies that you can safely call this function on both OBJand SOP asset nodesand get the same(equivalent) geometry display node back.SOP asset nodes will simply return themselves.
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Objects")
		static bool HoudiniGetDisplayGeoInfo(FHoudiniSession inhoudiniSession, int inNodeId, FHoudiniGeoInfo& outGeoInfo);
//Get ObjectInfo Sub data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Objects")
		static void HoudiniGetObjInfoSubData(const FHoudiniObjectInfo& inObjInfo, int& outNodeId, bool& bIsVisible);
//Get GeoInfo Sub data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Objects")
		static void HoudiniGetGeoInfoSubData(const FHoudiniGeoInfo& inGeoInfo, int& outSopNodeId, bool& isTemplated, bool& isDisplayGeo, int& outPartCount);
//Create a curve info struct.
	UFUNCTION(BlueprintCallable, BlueprintPure,meta = (AdvancedDisplay = "crvType,bHasKnots,knotCount,knotOrder,bIsPeriodic"), category = "zjhHoudiniUnrealPlugin | Helper")
		static FHoudiniCurveInfo HoudiniCreateCurveInfo(int crvCount, int pointCount, EHoudini_CurveType crvType = EHoudini_CurveType::Houdini_CURVETYPE_LINEAR, bool bHasKnots = false, int knotCount = 0, int knotOrder = 0, bool bIsPeriodic = false);
//Set meta-data for the curve mesh, including the curve type, order, and periodicity. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Curves")
		static bool HoudiniSetCurveInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, const FHoudiniCurveInfo& inCrvInfo);
//return  tempResult == HAPI_RESULT_SUCCESS;
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Curves")
		static bool HoudiniSetCurveCounts(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, const TArray<int>& curveCountsArray);
//Set single parm float value by name. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmFloatValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, float param_value);
//Set single parm int value by name. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmIntValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, int param_value);
//Set (push) a string value. We can only set a single value at a time because we want to avoid fixed size string buffers. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmStringValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_value, int param_id, int param_index);
//Set a node id parm value of an Op Path parameter. For example, This is how you connect the geometry output of an asset to the geometry input of another asset - whether the input is a parameter or a node input (the top of the node). Node inputs get converted top parameters in HAPI. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmNodeValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int valueNodeId);
//Set (push) an expression string. We can only set a single value at a time because we want to avoid fixed size string buffers. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmExpression(FHoudiniSession inhoudiniSession, int inNodeId, FString param_value, int param_id, int param_index);
//Set (push) an array of parameter float values. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmFloatValues(FHoudiniSession inhoudiniSession, int inNodeId, const TArray<float>& inValues, int start);
//Set (push) an array of parameter int values. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniSetParmIntValues(FHoudiniSession inhoudiniSession, int inNodeId, const TArray<int>& inValues, int start);
//Revert single parm by name to default. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniRevertParmToDefault(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index);
//Revert all instances of the parm by name to defaults. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniRevertParmToDefaults(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name);
//Get single parm float value by name. 
	UFUNCTION(BlueprintCallable,BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmFloatValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, float& outparam_value);
//Get single parm int value by name. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmIntValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, int& outparam_value);
//Get single parm string value by name. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmStringValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int param_index, bool bEvalueate, FString& outStrValue);
//Get a single node id parm value of an Op Path parameter. This is how you see which node is connected as an input for the current node (via parameter). 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmNodeValue(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int& outNodeId);
//Get (push) an array of parameter float values. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmFloatValues(FHoudiniSession inhoudiniSession, int inNodeId, TArray<float>& outValues, int startIndex, int count);
//Get (push) an array of parameter int values. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmIntValues(FHoudiniSession inhoudiniSession, int inNodeId, TArray<int>& outValues, int startIndex, int count);
//Fill an array of parameter string handles. These handles must be used in conjunction with HAPI_GetString() to get the actual string values. This is more efficient than calling HAPI_GetParmStringValue() individually for each parameter value. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmStringValues(FHoudiniSession inhoudiniSession, int inNodeId, bool bEvalueate, TArray<FString>& outStrValues, int startIndex, int count);
//Fill an array of HAPI_ParmInfo structs with parameter information from the asset instance node. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParameters(FHoudiniSession inhoudiniSession, int inNodeId, TArray<FHoudiniParamInfo>& parmInfoList, int count);
//Convenience function that checks on the value of the HAPI_ParmInfo::type field to tell you the underlying data type. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniParmInfo_IsInt(const FHoudiniParamInfo& inParmInfo);
//Convenience function that checks on the value of the HAPI_ParmInfo::type field to tell you the underlying data type. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniParmInfo_IsFloat(const FHoudiniParamInfo& inParmInfo);
//Convenience function that checks on the value of the HAPI_ParmInfo::type field to tell you the underlying data type. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniParmInfo_IsString(const FHoudiniParamInfo& inParmInfo);
//Convenience function. If the parameter can be represented by this data type,it returns HAPI_ParmInfo::size, and zero otherwise. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static int HoudiniGetParmInfoTypeValueCount(const FHoudiniParamInfo& inParmInfo, EHoudini_ParamValueType invalueType);
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static int HoudiniGetParmInfoValueIndex(const FHoudiniParamInfo& inParmInfo, EHoudini_ParamValueIndexType invalueIndexType);
//		Get the parm info of a parameter by name. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmInfoFromName(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, FHoudiniParamInfo& outParamInfo);
//		Get the parm id of a parameter by name. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmIdFromName(FHoudiniSession inhoudiniSession, int inNodeId, FString param_name, int& outParamId);
//Get the parm info of a parameter by parm id. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmInfo(FHoudiniSession inhoudiniSession, int inNodeId, int inParmId, FHoudiniParamInfo& outParamInfo);
//Get ChoiceCount
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static int HoudiniGetChoiceCount(const FHoudiniParamInfo& inParmInfo);
//Fill an array of HAPI_ParmChoiceInfo structs with parameter choice list information from the asset instance node. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmChoiceLists(FHoudiniSession inhoudiniSession, int inNodeId, TArray<FHoudiniParmChoiceInfo>& outParmChoiceInfos, int start, int count);
//Get ParmChoiceInfo sub data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniGetParmChoiceInfoSubData(FHoudiniSession inhoudiniSession, const TArray<FHoudiniParmChoiceInfo>& inChoiceInfo, TArray<FString>& outValues, TArray<FString>& outLabels);
//Parameter has no underlying No data type. Examples of this are UI items such as folder lists and separators. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static bool HoudiniParmInfoIsNonValue(const FHoudiniParamInfo& inParmInfo);
//Get multiParm sub data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Param")
		static void HoudiniGetMultiParmSubData(const FHoudiniParamInfo& inParmInfo, int& outinstanceNum, int& outinstanceCount, int& outinstanceLength, int& outinstanceStartOffset, EHoudini_RampType& outRampType);
//Get the part ids that this instancer part is instancing. 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Getters")
		static bool HoudiniGetInstancedPartIds(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, TArray<int>& outPartIdList, int count);
//Get the instancer part's list of transforms on which to instance the instanced parts you got from HoudiniGetInstancedPartIds(). 
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhHoudiniUnrealPlugin | Getters")
		static bool HoudiniGetInstancerPartTransforms(FHoudiniSession inhoudiniSession, int inNodeId, int inPartId, TArray<FTransform>& outTransforms, int count);


private:
	static FHoudiniSession globalHSession;
	static FString ToString(FHoudiniSession inhoudiniSession, HAPI_StringHandle inAssethandle);
	template<typename T>
	static FString HoudiniEnumToString(const T enumValue);
	template<typename EnumType>
	static EnumType HoudiniStringToEnum(const FString& instr);
};

template<typename T>
inline FString UHoudiniEngineBPLibrary::HoudiniEnumToString(const T enumValue)
{
	FString name = StaticEnum<T>()->GetNameStringByValue(static_cast<__underlying_type(T)>(enumValue));
	check(name.Len() != 0);
	return name;
}

template<typename EnumType>
inline EnumType UHoudiniEngineBPLibrary::HoudiniStringToEnum(const FString& instr)
{
	return static_cast<EnumType>(StaticEnum<EnumType>()->GetValueByName(FName(*instr)));
}
