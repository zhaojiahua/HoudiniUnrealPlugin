#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Json.h"
#include "ForRZObject.generated.h"

UCLASS()
class HOUDINIUNREALPLUGIN_API UForRZObject : public UObject
{
	GENERATED_BODY()
	
public:
//		由结构体生成JSON并打印
	UFUNCTION(BlueprintCallable, category = "zjhAddFuns | Json")
		static void GenerateAndPrintJson();
};

struct FObjectTest
{
	int32 index_int;
	float  index_float;
};

struct FXXXX
{
	FXXXX() :name("zhangsan"), age(30), bMarry(false), fff(0.0f), AlientID(TEXT("gamma")), testobjPtr(nullptr)
	{};
	FString name;
	int32 age;
	bool bMarry;
	float fff;
	FString AlientID;
	FObjectTest testobj;
	FObjectTest* testobjPtr;
	TArray<FXXXX> selfArray;

	FString ToString()
	{
		FString outJsonString;
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>>JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&outJsonString);
		ToString(JsonWriter, *this);
		JsonWriter->Close();
		return outJsonString;
	}

	void ToString(TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>>JsonWriter, FXXXX& infx)
	{
		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("name"), infx.name);
			JsonWriter->WriteValue(TEXT("age"), infx.age);
			JsonWriter->WriteValue(TEXT("bMarry"), infx.bMarry);
			JsonWriter->WriteValue(TEXT("fff"), infx.fff);
			JsonWriter->WriteValue(TEXT("AlientID"), infx.AlientID);
			JsonWriter->WriteObjectStart("testobj");
			{
				JsonWriter->WriteValue(TEXT("index_int"), infx.testobj.index_int);
				JsonWriter->WriteValue(TEXT("index_float"), infx.testobj.index_float);
			}
			JsonWriter->WriteObjectEnd();
			if (testobjPtr)
			{
				JsonWriter->WriteObjectStart("testobj");
				{
					JsonWriter->WriteValue(TEXT("index_int"), infx.testobjPtr->index_int);
					JsonWriter->WriteValue(TEXT("index_float"), infx.testobjPtr->index_float);
				}
				JsonWriter->WriteObjectEnd();
			}
			JsonWriter->WriteArrayStart("selfArray");
			for (auto& temp : infx.selfArray)
			{
				ToString(JsonWriter, temp);
			}
			JsonWriter->WriteArrayEnd();
		}
		JsonWriter->WriteObjectEnd();
	}
};