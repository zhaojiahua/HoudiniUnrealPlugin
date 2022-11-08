// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "HAPIToUnrealCommonBridge.h"
#include "HoudiniEngineUtilityLibrary.generated.h"


UCLASS()
class HOUDINIENGINE_API UHoudiniEngineUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
//get mesh Unreal mesh data
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | mesh")
	static bool GetUnrealMeshData(UStaticMesh* staticMesh, TArray<FVector>& pointList, TArray<int>& vertexList, TArray<int>& faceList, TArray<FVector>& normalList, TArray<FVector>& tangentlList, TArray<FVector2D>& uvList, TArray<int>& sectionList, TArray<UMaterialInterface*>& materialList);
//把FVector类型的列表转换成floatList形式存储
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
	static bool FVectorToFloatList(const TArray<FVector>& inVector, TArray<float>& outFloatArray, bool bSwitchYZAxis = true, float scaleMulti = 0.01);		//Unreal里面的默认单位是米,houdini里面的单位是厘米
//把FVector类型的列表转换成floatList形式存储
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static bool FVector2DToFloatList(const TArray<FVector2D>& inVector2D, TArray<float>& outFloatArray, bool flipY = true);
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static bool FloatListToFVectorList(TArray<FVector>& outVector, const TArray<float>& inFloatArray, bool bSwitchYZAxis = true, float scaleMulti = 100);
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static bool FloatListToProMeshTangentList(TArray<FProcMeshTangent>& outTangent, const TArray<float>& inFloatArray, bool bSwitchYZAxis = true, bool bFlipTangentY = false);
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static bool FloatListToFVector2DList(TArray<FVector2D>& outVector, const TArray<float>& inFloatArray, bool bFlipY = true);
	UFUNCTION(BlueprintCallable, category = "zjhAddFuns | Unreal | Utility")
		static bool ReverseVertexList(UPARAM(ref) TArray<int>& inVertexList);
// 分割顶点列表
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Mesh")
		static bool SplitVertexList(const TArray<int>& inVertexList, const TArray<int>& inSectionList, TArray<FVertexList>& outSplitedVertexList);
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Mesh")
		static bool SplitVertexListByStr(const TArray<int>& inVertexList, const TArray<FString>& inSectionNameList, TArray<FVertexList>& outSplitedVertexList);
//Unreal Transform To Houdini Transform
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static FHoudiniTranform UnrealTransformToHoudiniTransform(const FTransform& inUETransform);
//Unreal Transform To Houdini Euler Transform
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static FHoudiniTranformEuler UnrealTransformToHoudiniTransformEuler(const FTransform& inUETransform);
//Houdini Transform To Unreal Transform
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static FTransform HoudiniTransformToUnrealTransform(const FHoudiniTranform& inHouTransform);
//Houdini TransformEuler To Unreal Transform
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "zjhAddFuns | Unreal | Utility")
		static FTransform HoudiniTransformEulerToUnrealTransform(FHoudiniSession inhoudiniSession, const FHoudiniTranformEuler& inHouTransformEuler);

};
