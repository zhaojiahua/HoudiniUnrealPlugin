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
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Creates a Thrift RPC session using a Windows named pipe or a Unix domain socket as transport. "), category = "zjhHoudiniUnrealPlugin | Sessions")
		static FHoudiniSession StartServerAndCreateSession(FHoudiniSession inSession);
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Creates options which affect how nodes are cooked. "), category = "zjhHoudiniUnrealPlugin")
		static FHoudiniCookOption CreateHoudiniCookOption();
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
	UFUNCTION(BlueprintCallable,BlueprintPure, category = "zjhHoudiniUnrealPlugin | Attributes")
		static void HoudiniGetGeoFromPartInfo(const FHoudiniPartInfo& intPartInfo, int& faceCount, int& pointCount);
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
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Objects")
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

private:
	static FString ToString(FHoudiniSession inhoudiniSession, HAPI_StringHandle inAssethandle);
	template<typename T>
	static FString HoudiniEnumToString(const T enumValue);
};

template<typename T>
inline FString UHoudiniEngineBPLibrary::HoudiniEnumToString(const T enumValue)
{
	FString name = StaticEnum<T>()->GetNameStringByValue(static_cast<__underlying_type(T)>(enumValue));
	check(name.Len() != 0);
	return name;
}
