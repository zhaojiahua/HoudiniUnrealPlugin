
#include "HoudiniEngineUtilityLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <HoudiniEngineBPLibrary.h>
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Curves/CurveLinearColor.h"


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

bool UHoudiniEngineUtilityLibrary::FVector2DToFloatList(const TArray<FVector2D>& inVector2D, TArray<float>& outFloatArray, bool flipY)
{
	if (inVector2D.Num() == 0)return false;
	for (int i = 0; i < inVector2D.Num(); i++)
	{
		outFloatArray.Add(inVector2D[i].X);
		flipY ? outFloatArray.Add(1-inVector2D[i].Y) : outFloatArray.Add(inVector2D[i].Y);
		outFloatArray.Add(0);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::FloatListToFVectorList(TArray<FVector>& outVector, const TArray<float>& inFloatArray, bool bSwitchYZAxis, float scaleMulti)
{
	if (inFloatArray.Num() == 0 || inFloatArray.Num() % 3 != 0)	return false;
	for (int i = 0; i < inFloatArray.Num() / 3; i++)
	{
		FVector tempVector;
		tempVector.X = inFloatArray[3*i];
		int indexY = bSwitchYZAxis ? 2 : 1;
		int indexZ = bSwitchYZAxis ? 1 : 2;
		tempVector.Y = inFloatArray[3 * i + indexY];
		tempVector.Z = inFloatArray[3 * i + indexZ];
		outVector.Add(tempVector * scaleMulti);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::FloatListToProMeshTangentList(TArray<FProcMeshTangent>& outTangent, const TArray<float>& inFloatArray, bool bSwitchYZAxis, bool bFlipTangentY)
{
	if (inFloatArray.Num() == 0 || inFloatArray.Num() % 3 != 0)	return false;
	for (int i = 0; i < inFloatArray.Num() / 3; i++)
	{
		FVector tempVector;
		tempVector.X = inFloatArray[3 * i];
		int indexY = bSwitchYZAxis ? 2 : 1;
		int indexZ = bSwitchYZAxis ? 1 : 2;
		tempVector.Y = inFloatArray[3 * i + indexY];
		tempVector.Z = inFloatArray[3 * i + indexZ];
		outTangent.Add(FProcMeshTangent(tempVector, bFlipTangentY));
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::FloatListToFVector2DList(TArray<FVector2D>& outVector, const TArray<float>& inFloatArray, bool bFlipY)
{
	if (inFloatArray.Num() == 0 || inFloatArray.Num() % 3 != 0)	return false;
	for (int i = 0; i < inFloatArray.Num() / 3; i++)
	{
		FVector2D tempVector2D(inFloatArray[i * 3], bFlipY ? 1 - inFloatArray[i * 3 + 1] : inFloatArray[i * 3 + 1]);
		outVector.Add(tempVector2D);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::RotatorToFloatList(const TArray<FRotator>& rotList, TArray<float>& outFloatArray, bool bNegativeW)
{
	if (rotList.Num() <= 0)	return false;
	outFloatArray.SetNumUninitialized(rotList.Num() * 4);
	for (int i = 0; i < rotList.Num(); i++)
	{
		FQuat tempQuat = rotList[i].Quaternion();
		outFloatArray[i * 4] = tempQuat.X;
		outFloatArray[i * 4+1] = tempQuat.Z;
		outFloatArray[i * 4+2] = tempQuat.Y;
		outFloatArray[i * 4 + 3] = bNegativeW ? -tempQuat.W : tempQuat.W;
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::ReverseVertexList(UPARAM(ref) TArray<int>& inVertexList)
{
	if (inVertexList.Num() == 0 || inVertexList.Num() % 3 != 0)	return false;
	//for (int i = 0; i < inVertexList.Num() / 3; i++)
	//{
	//	Swap(inVertexList[i * 3 + 1], inVertexList[i * 3 + 2]);
	//}
	ParallelFor(inVertexList.Num() / 3, [&inVertexList](int i) {Swap(inVertexList[i * 3], inVertexList[i * 3 + 1]); });
	return true;
}

bool UHoudiniEngineUtilityLibrary::SplitVertexList(const TArray<int>& inVertexList, const TArray<int>& inSectionList, TArray<FVertexList>& outSplitedVertexList)
{
	if (inVertexList.Num() == 0 || inSectionList.Num() == 0 || inVertexList.Num() != inSectionList.Num()*3)	return false;
	int maxValueIndex = -1;
	int maxValue = 0;
	UKismetMathLibrary::MaxOfIntArray(inSectionList, maxValueIndex, maxValue);
	outSplitedVertexList.SetNum(maxValue + 1);
	for (int i = 0; i < inVertexList.Num(); i++)
	{
		outSplitedVertexList[inSectionList[i / 3]].vertexList.Add(inVertexList[i]);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::SplitVertexListByStr(const TArray<int>& inVertexList, const TArray<FString>& inSectionNameList, TArray<FVertexList>& outSplitedVertexList)
{
	if (inVertexList.Num() == 0 || inSectionNameList.Num() == 0 || inVertexList.Num() != inSectionNameList.Num() * 3)	return false;
	TArray<FString> tempUniqueStrList;
	for (auto& item : inSectionNameList)
	{
		tempUniqueStrList.AddUnique(item);
	}
	outSplitedVertexList.SetNum(tempUniqueStrList.Num());
	for (int i = 0; i < inVertexList.Num(); i++)
	{
		int tempIndex = tempUniqueStrList.Find(inSectionNameList[i / 3]);
		outSplitedVertexList[tempIndex].vertexList.Add(inVertexList[i]);
	}
	return true;
}

FHoudiniTranform UHoudiniEngineUtilityLibrary::UnrealTransformToHoudiniTransform(const FTransform& inUETransform)
{
	float transformScaleFactor = 100.0f;
	HAPI_Transform houdiniTransform;
	HAPI_Transform_Init(&houdiniTransform);
	houdiniTransform.rstOrder = HAPI_SRT;

	FQuat unrealRotateQuat = inUETransform.GetRotation();
	FVector unrealTranslateVector = inUETransform.GetTranslation() / transformScaleFactor;
	FVector unrealScaleVector = inUETransform.GetScale3D();

	houdiniTransform.rotationQuaternion[0] = -unrealRotateQuat.X;
	houdiniTransform.rotationQuaternion[1] = -unrealRotateQuat.Z;
	houdiniTransform.rotationQuaternion[2] = -unrealRotateQuat.Y;
	houdiniTransform.rotationQuaternion[3] = unrealRotateQuat.W;
	houdiniTransform.position[0] = unrealTranslateVector.X;
	houdiniTransform.position[1] = unrealTranslateVector.Z;
	houdiniTransform.position[2] = unrealTranslateVector.Y;
	houdiniTransform.scale[0] = unrealScaleVector.X;
	houdiniTransform.scale[1] = unrealScaleVector.Z;
	houdiniTransform.scale[2] = unrealScaleVector.Y;

	FHoudiniTranform tempHoudiniTransform;
	tempHoudiniTransform.houTransform = houdiniTransform;
	return tempHoudiniTransform;
}

FHoudiniTranformEuler UHoudiniEngineUtilityLibrary::UnrealTransformToHoudiniTransformEuler(const FTransform& inUETransform)
{
	float transformScaleFactor = 100.0f;
	HAPI_TransformEuler houdiniTransformEuler;
	HAPI_TransformEuler_Init(&houdiniTransformEuler);
	houdiniTransformEuler.rstOrder = HAPI_SRT;
	//houdiniTransformEuler.rotationEuler = HAPI_XYZ;

	FQuat unrealRotateQuat = inUETransform.GetRotation();
	FVector unrealTranslateVector = inUETransform.GetTranslation() / transformScaleFactor;
	FVector unrealScaleVector = inUETransform.GetScale3D();

	Swap(unrealRotateQuat.Y, unrealRotateQuat.Z);
	unrealRotateQuat.W = -unrealRotateQuat.W;

	houdiniTransformEuler.rotationEuler[0] = -unrealRotateQuat.Rotator().Roll;
	houdiniTransformEuler.rotationEuler[1] = -unrealRotateQuat.Rotator().Pitch;
	houdiniTransformEuler.rotationEuler[2] = unrealRotateQuat.Rotator().Yaw;
	houdiniTransformEuler.position[0] = unrealTranslateVector.X;
	houdiniTransformEuler.position[1] = unrealTranslateVector.Z;
	houdiniTransformEuler.position[2] = unrealTranslateVector.Y;
	houdiniTransformEuler.scale[0] = unrealScaleVector.X;
	houdiniTransformEuler.scale[1] = unrealScaleVector.Z;
	houdiniTransformEuler.scale[2] = unrealScaleVector.Y;

	FHoudiniTranformEuler tempHoudiniTransformEuler;
	tempHoudiniTransformEuler.houTransformEuler = houdiniTransformEuler;
	return tempHoudiniTransformEuler;
}

FTransform UHoudiniEngineUtilityLibrary::HoudiniTransformToUnrealTransform(const FHoudiniTranform& inHouTransform)
{
	HAPI_Transform houTransform = inHouTransform.houTransform;
	float transformScaleFactor = 100.0f;
	FQuat ueRotation(houTransform.rotationQuaternion[0], houTransform.rotationQuaternion[2], houTransform.rotationQuaternion[1], houTransform.rotationQuaternion[3]);
	FVector ueTranslate(houTransform.position[0], houTransform.position[2], houTransform.position[1]);
	ueTranslate *= transformScaleFactor;
	FVector ueScale(houTransform.scale[0], houTransform.scale[2], houTransform.scale[1]);
	return FTransform(ueRotation, ueTranslate, ueScale);
}

FTransform UHoudiniEngineUtilityLibrary::HoudiniTransformEulerToUnrealTransform(FHoudiniSession inhoudiniSession, const FHoudiniTranformEuler& inHouTransformEuler)
{
	if (!UHoudiniEngineBPLibrary::HoudiniSessionIsValid(inhoudiniSession)) return FTransform::Identity;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	HAPI_TransformEuler houTransformEuler = inHouTransformEuler.houTransformEuler;
	float matrix[16];
	HAPI_ConvertTransformEulerToMatrix(&houSession, &houTransformEuler, matrix);	//把EulerTransform转换成4*4矩阵
	HAPI_Transform houTransformQuat;
	HAPI_Transform_Init(&houTransformQuat);
	HAPI_ConvertMatrixToQuat(&houSession, matrix, HAPI_SRT, &houTransformQuat);	//再把4*4矩阵转成TransformQuat

	FHoudiniTranform tempHoudiniTransfrom;
	tempHoudiniTransfrom.houTransform = houTransformQuat;
	return HoudiniTransformToUnrealTransform(tempHoudiniTransfrom);	//最后调用上面的函数即可
}

bool UHoudiniEngineUtilityLibrary::GetFloatCurveKeyframesData(UCurveFloat* incurveFloat, TArray<float>& times, TArray<float>& values, TArray<TEnumAsByte<ERichCurveInterpMode>>& curveModes)
{
	if (!incurveFloat || incurveFloat->IsPendingKill())	return false;
	FRichCurve& crvDatas = incurveFloat->FloatCurve;
	int keynum = crvDatas.GetNumKeys();
	if (!keynum) return false;
	for (int i = 0; i < keynum; i++)
	{
		float tempTime = crvDatas.Keys[i].Time;
		float tempValue = crvDatas.Keys[i].Value;
		auto mode = crvDatas.Keys[i].InterpMode;
		times.Add(tempTime);
		values.Add(tempValue);
		curveModes.Add(mode);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::GetColorCurveKeyframesData(UCurveLinearColor* incurveLinearColor, TArray<float>& times, TArray<FLinearColor>& colors, TArray<TEnumAsByte<ERichCurveInterpMode>>& curveModes)
{
	if (!incurveLinearColor || incurveLinearColor->IsPendingKill())	return false;
	auto& crvDatas = incurveLinearColor->FloatCurves;
	int keynum = crvDatas[0].GetNumKeys();	//这里注意rgb和Alpha通道是分开的,它们的帧数可能不一样
	if (!keynum) return false;
	for (int i = 0; i < keynum; i++)
	{
		float tempTime = crvDatas[0].Keys[i].Time;
		float tempValue_R = crvDatas[0].Keys[i].Value;
		float tempValue_G = crvDatas[1].Keys[i].Value;
		float tempValue_B = crvDatas[2].Keys[i].Value;
		FLinearColor tempLinearColor(tempValue_R, tempValue_G, tempValue_B);
		auto mode = crvDatas[0].Keys[i].InterpMode;
		times.Emplace(tempTime);
		colors.Emplace(tempLinearColor);
		curveModes.Emplace(mode);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::FloatListToTransformList(FHoudiniSession inhoudiniSession, const TArray<float>& floatList, TArray<FTransform>& outTransformList)
{
	if (!UHoudiniEngineBPLibrary::HoudiniSessionIsValid(inhoudiniSession)) return false;
	HAPI_Session houSession = inhoudiniSession.ToHAPI_Session();
	if (floatList.Num() == 0 || floatList.Num() % 16 != 0)return false;
	for (int i = 0; i < floatList.Num()/16; i++)
	{
		float houMatrix[16];
		for (int j = 0; j < 16; j++)
		{
			houMatrix[j] = floatList[i * 16 + j];
		}
		HAPI_Transform houTansformQuat;
		HAPI_Transform_Init(&houTansformQuat);
		HAPI_ConvertMatrixToQuat(&houSession, houMatrix, HAPI_SRT, &houTansformQuat);
		FHoudiniTranform temHoudiniTransform;
		temHoudiniTransform.houTransform = houTansformQuat;
		FTransform tempTransform = HoudiniTransformToUnrealTransform(temHoudiniTransform);
		outTransformList.Add(tempTransform);
	}
	return true;
}

bool UHoudiniEngineUtilityLibrary::SplitTransformListByString(const TArray<FTransform>& inTransformList, const TArray<FString>& AttrNames, TArray<FUnrealSplitedTransforms>& outSplitedTransforms)
{
	if (inTransformList.Num() == 0 || AttrNames.Num() == 0 || inTransformList.Num() != AttrNames.Num()) return false;
	TArray<FString> uniqueStrName;
	for (auto item : AttrNames)
	{
		uniqueStrName.AddUnique(item);
	}
	outSplitedTransforms.SetNumZeroed(uniqueStrName.Num());
	for (int i = 0; i < inTransformList.Num(); i++)
	{
		int index = uniqueStrName.Find(AttrNames[i]);
		outSplitedTransforms[index].transformList.Add(inTransformList[i]);
	}
	for (int j = 0; j < uniqueStrName.Num(); j++)
	{
		outSplitedTransforms[j].partName = uniqueStrName[j];
	}
	return true;
}

void UHoudiniEngineUtilityLibrary::HoudiniNotification(FString inMessageStr, float duringTime, bool bUseLargeFont)
{
	FNotificationInfo	messageInfo(FText::FromString(inMessageStr));
	messageInfo.bFireAndForget = true;
	messageInfo.ExpireDuration = duringTime;
	messageInfo.bUseLargeFont = bUseLargeFont;
	messageInfo.bUseSuccessFailIcons = false;
	FSlateNotificationManager::Get().AddNotification(messageInfo)->SetCompletionState(SNotificationItem::CS_Success);
}

