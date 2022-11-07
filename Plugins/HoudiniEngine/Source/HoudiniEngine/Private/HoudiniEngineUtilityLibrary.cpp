
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

	FPositionVertexBuffer* positionVertexBuffer = &staticLodResourceVertexBuffer->PositionVertexBuffer;		//���Ƶ�Points�Ļ���
	int pointNum = positionVertexBuffer->GetNumVertices();	//���Ƶ�Points������
	int indicesNum = staticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetNumIndices();	//���������
	FIndexArrayView indexArray = staticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetArrayView();		//�����б�
	int faceCount = staticMesh->GetRenderData()->LODResources[0].GetNumTriangles();		//��ȡ�����������
	auto sectionsArray = staticMesh->GetRenderData()->LODResources[0].Sections;		//��ȡģ�͵�sections

	pointList.SetNumUninitialized(pointNum);
	vertexList.SetNumUninitialized(indicesNum);
	normalList.SetNumUninitialized(pointNum);
	tangentlList.SetNumUninitialized(pointNum);
	uvList.SetNumUninitialized(pointNum);
	materialList.SetNumUninitialized(sectionsArray.Num());
	sectionList.SetNumUninitialized(faceCount);
	faceList.Init(3, faceCount);	//����������,��������ÿ����ĵ�������3
	
	//��CtrPoints�������
	for (int i = 0; i < pointNum; i++)
	{
		pointList[i] = positionVertexBuffer->VertexPosition(i);
		normalList[i] = staticLodResourceVertexBuffer->StaticMeshVertexBuffer.VertexTangentZ(i);	//������Z�ķ�����Ǹö���ķ��߷���
		tangentlList[i] = staticLodResourceVertexBuffer->StaticMeshVertexBuffer.VertexTangentX(i);
		uvList[i] = staticLodResourceVertexBuffer->StaticMeshVertexBuffer.GetVertexUV(i, 0);
	}

	//��indices�йص�����
	for (int j = 0; j < indicesNum; j++)
	{
		vertexList[j] = indexArray[j];
	}

	//��Section��ص�����
	for (int k = 0; k < sectionsArray.Num(); k++)
	{
		materialList[k] = staticMesh->GetMaterial(sectionsArray[k].MaterialIndex);	//��ȡ����
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
