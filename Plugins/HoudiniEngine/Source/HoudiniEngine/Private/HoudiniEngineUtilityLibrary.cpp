
#include "HoudiniEngineUtilityLibrary.h"

bool UHoudiniEngineUtilityLibrary::GetUnrealMeshData(UStaticMesh* staticMesh, TArray<FVector>& pointList, TArray<int>& vertexList, TArray<int>& faceList, TArray<FVector>& normalList, TArray<FVector>& tangentlList, TArray<FVector2D>& uvList, TArray<int>& sectionList, TArray<UMaterialInterface*>& materialList)
{
	if (staticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("the static mesh is none"));
		return false;
	}
	if (!staticMesh->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("the static mesh is not valid"));
		return false;
	}
	if (!staticMesh->GetRenderData() || staticMesh->GetRenderData()->LODResources.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("the static mesh is empty"));
		return false;
	}

	FStaticMeshVertexBuffers* staticLodResourceVertexBuffer = &staticMesh->GetRenderData()->LODResources[0].VertexBuffers;

	FPositionVertexBuffer* positionVertexBuffer = &staticLodResourceVertexBuffer->PositionVertexBuffer;		//控制点Points的缓存
	int pointNum = positionVertexBuffer->GetNumVertices();	//控制点Points的数量
	int indicesNum = staticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetNumIndices();	//顶点的数量
	FIndexArrayView indexArray = staticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetArrayView();		//顶点列表
	int faceCount = staticMesh->GetRenderData()->LODResources[0].GetNumTriangles();		//获取三角面的数量
	auto sectionsArray = staticMesh->GetRenderData()->LODResources[0].Sections;		//获取模型的sections

	pointList.SetNumUninitialized(pointNum);
	vertexList.SetNumUninitialized(indicesNum);
	normalList.SetNumUninitialized(pointNum);
	tangentlList.SetNumUninitialized(pointNum);
	uvList.SetNumUninitialized(pointNum);
	materialList.SetNumUninitialized(sectionsArray.Num());
	sectionList.SetNumUninitialized(faceCount);
	faceList.Init(3, faceCount);	//都是三角面,所以设置每个面的点数都是3
	
	//和CtrPoints相关数据
	for (int i = 0; i < pointNum; i++)
	{
		pointList[i] = positionVertexBuffer->VertexPosition(i);
		normalList[i] = staticLodResourceVertexBuffer->StaticMeshVertexBuffer.VertexTangentZ(i);	//点切线Z的方向就是该顶点的法线方向
		tangentlList[i] = staticLodResourceVertexBuffer->StaticMeshVertexBuffer.VertexTangentX(i);
		uvList[i] = staticLodResourceVertexBuffer->StaticMeshVertexBuffer.GetVertexUV(i, 0);
	}

	//和indices有关的数据
	for (int j = 0; j < indicesNum; j++)
	{
		vertexList[j] = indexArray[j];
	}

	//和Section相关的数据
	for (int k = 0; k < sectionsArray.Num(); k++)
	{
		materialList[k] = staticMesh->GetMaterial(sectionsArray[k].MaterialIndex);	//获取材质
		int triangleNum = sectionsArray[k].NumTriangles;
		for (int tt = 0; tt < triangleNum; tt++)
		{
			int firstIndex = sectionsArray[k].FirstIndex;
			sectionList[tt + firstIndex / 3] = k;
		}
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::FVectorToFloatList(const TArray<FVector>& inVector, TArray<float>& outFloatArray, bool bSwitchYZAxis, float scaleMulti)
{
	if (inVector.Num() == 0)return false;
	for (int i = 0; i < inVector.Num(); i++)
	{
		outFloatArray.Add(inVector[i].X * scaleMulti);
		bSwitchYZAxis ? outFloatArray.Add(inVector[i].Z * scaleMulti) : outFloatArray.Add(inVector[i].Y * scaleMulti);
		bSwitchYZAxis ? outFloatArray.Add(inVector[i].Y * scaleMulti) : outFloatArray.Add(inVector[i].Z * scaleMulti);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::FVector2DToFloatList(const TArray<FVector2D>& inVector2D, TArray<float>& outFloatArray, float scaleMulti)
{
	if (inVector2D.Num() == 0)return false;
	for (int i = 0; i < inVector2D.Num(); i++)
	{
		outFloatArray.Add(inVector2D[i].X * scaleMulti);
		outFloatArray.Add(inVector2D[i].Y * scaleMulti);
		outFloatArray.Add(0);
	}
	return true;
}
