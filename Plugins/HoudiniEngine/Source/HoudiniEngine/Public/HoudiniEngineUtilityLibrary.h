// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
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
	static bool FVector2DToFloatList(const TArray<FVector2D>& inVector2D, TArray<float>& outFloatArray, float scaleMulti = 0.01);

};
