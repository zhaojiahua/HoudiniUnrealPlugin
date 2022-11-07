
#include "ForRZObject.h"

void UForRZObject::GenerateAndPrintJson()
{
	FXXXX fxObj;
	fxObj.name = TEXT("lisi");
	fxObj.age = 30;
	fxObj.AlientID = TEXT("NULL");
	fxObj.bMarry = false;
	fxObj.fff = 10.0;
	fxObj.testobj.index_int = 100;
	fxObj.testobj.index_float = 100.1359f;
	fxObj.testobjPtr = new FObjectTest();
	fxObj.testobjPtr->index_int = 321;
	fxObj.testobjPtr->index_float = 132.0321;

	FXXXX fxObj2 = fxObj;
	fxObj.selfArray.Add(fxObj2);
	fxObj.selfArray.Add(fxObj2);

	FString forJsonString = fxObj.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *forJsonString);
}
