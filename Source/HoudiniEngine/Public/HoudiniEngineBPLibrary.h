// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HAPIToUnrealCommonBridge.h"
#include "HoudiniEngineBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
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
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Create the asset manager, set up environment variables, and initialize the main Houdini scene. No license check is done during this step. Only when you try to load an asset library (OTL) do we actually check for licenses. "), category = "zjhHoudiniUnrealPlugin | Sessions")
		static void InitializeHoudiniSession(FHoudiniSession inhoudiniSession, const FHoudiniCookOption& inhoudiniCookOption);
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "CChecks whether the session identified by HAPI_Session::id is a valid session opened in the implementation identified by HAPI_Session::type.  "), category = "zjhHoudiniUnrealPlugin | Sessions")
		static bool HoudiniSessionIsValid(FHoudiniSession inhoudiniSession);
	//Loads a Houdini asset library (OTL) from a .otl file. It does NOT create anything inside the Houdini scene. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Assets")
		static bool HoudiniLoadAssetLibraryFromFile(FHoudiniSession inhoudiniSession, FString otlFilePath, bool allowOverWrite, int& assetLibraryId);
	//Get the names of the assets contained in an asset library. 
	UFUNCTION(BlueprintCallable, category = "zjhHoudiniUnrealPlugin | Assets")
		static bool GetAvailableAssetOptionName(FHoudiniSession inhoudiniSession, int inlibrary_id, FString& fullOptionName, FString& label);
private:
	static FString ToString(FHoudiniSession inhoudiniSession, HAPI_StringHandle inAssethandle);
};
