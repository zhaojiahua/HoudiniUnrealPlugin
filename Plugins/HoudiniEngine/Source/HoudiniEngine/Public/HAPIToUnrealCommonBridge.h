#pragma once
#include "CoreMinimal.h"
#include "HAPI/HAPI.h"
#include "HAPIToUnrealCommonBridge.generated.h"

/// ////////////////////////////////////////////////////////////////////////////////////////////// ENUM
UENUM(BlueprintType)
enum class EHoudini_SessionType : uint8
{
    Houdini_SESSION_INPROCESS,
    Houdini_SESSION_THRIFT,
    Houdini_SESSION_CUSTOM1,
    Houdini_SESSION_CUSTOM2,
    Houdini_SESSION_CUSTOM3,
    Houdini_SESSION_MAX
};

UENUM(BlueprintType)
enum class EHoudini_NodeType : uint8
{
    Houdini_NODETYPE_ANY,
    Houdini_NODETYPE_NONE,
    Houdini_NODETYPE_OBJ,
    Houdini_NODETYPE_SOP,
    Houdini_NODETYPE_CHOP,
    Houdini_NODETYPE_ROP,
    Houdini_NODETYPE_SHOP,
    Houdini_NODETYPE_COP,
    Houdini_NODETYPE_VOP,
    Houdini_NODETYPE_DOP,
    Houdini_NODETYPE_TOP
};

UENUM(BlueprintType)
enum class EHoudini_NodeFlag : uint8
{
    Houdini_NODEFLAGS_ANY,
    Houdini_NODEFLAGS_NONE,
    Houdini_NODEFLAGS_DISPLAY,
    Houdini_NODEFLAGS_RENDER,
    Houdini_NODEFLAGS_TEMPLATED,
    Houdini_NODEFLAGS_LOCKED,
    Houdini_NODEFLAGS_EDITABLE,
    Houdini_NODEFLAGS_BYPASS,
    Houdini_NODEFLAGS_NETWORK,
    Houdini_NODEFLAGS_OBJ_GEOMETRY,
    Houdini_NODEFLAGS_OBJ_CAMERA,
    Houdini_NODEFLAGS_OBJ_LIGHT,
    Houdini_NODEFLAGS_OBJ_SUBNET,
    Houdini_NODEFLAGS_SOP_CURVE,
    Houdini_NODEFLAGS_SOP_GUIDE,
    Houdini_NODEFLAGS_TOP_NONSCHEDULER
};
UENUM(BlueprintType)
enum class EHoudini_PartType : uint8
{
    Houdini_PARTTYPE_INVALID,
    Houdini_PARTTYPE_MESH,
    Houdini_PARTTYPE_CURVE,
    Houdini_PARTTYPE_VOLUME,
    Houdini_PARTTYPE_INSTANCER,
    Houdini_PARTTYPE_BOX,
    Houdini_PARTTYPE_SPHERE,
    Houdini_PARTTYPE_MAX
};

UENUM(BlueprintType)
enum class EHoudini_AttributeOwner : uint8
{
    Houdini_ATTROWNER_INVALID,
    Houdini_ATTROWNER_VERTEX,
    Houdini_ATTROWNER_POINT,
    Houdini_ATTROWNER_PRIM,
    Houdini_ATTROWNER_DETAIL,
    Houdini_ATTROWNER_MAX
};

UENUM(BlueprintType)
enum class EHoudini_StorageType : uint8
{
    Houdini_STORAGETYPE_INVALID ,
    Houdini_STORAGETYPE_INT,
    Houdini_STORAGETYPE_INT64,
    Houdini_STORAGETYPE_FLOAT,
    Houdini_STORAGETYPE_FLOAT64,
    Houdini_STORAGETYPE_STRING,
    Houdini_STORAGETYPE_MAX
};

UENUM(BlueprintType)
enum class EHoudini_CurveType : uint8
{
    Houdini_CURVETYPE_INVALID,
    Houdini_CURVETYPE_LINEAR,
    Houdini_CURVETYPE_NURBS,
    Houdini_CURVETYPE_BEZIER,
    Houdini_CURVETYPE_MAX
};

UENUM(BlueprintType)
enum class EHoudini_ParamValueType : uint8
{
    Houdini_ParamValue_float UMETA(DisplayName = "float"),
    Houdini_ParamValue_int UMETA(DisplayName = "int"),
    Houdini_ParamValue_string UMETA(DisplayName = "string")
};

UENUM(BlueprintType)
enum class EHoudini_ParamValueIndexType : uint8
{
    Houdini_ParamValueIndex_float UMETA(DisplayName = "float"),
    Houdini_ParamValueIndex_int UMETA(DisplayName = "int"),
    Houdini_ParamValueIndex_string UMETA(DisplayName = "string"),
    Houdini_ParamValueIndex_choice UMETA(DisplayName = "choice")
};

UENUM(BlueprintType)
enum class EHoudini_RampType : uint8
{
    Houdini_RampType_INVALID UMETA(DisplayName = "invalid"),
    Houdini_RampType_FLOAT UMETA(DisplayName = "float"),
    Houdini_RampType_COLOR UMETA(DisplayName = "color"),
    Houdini_RampType_MAX UMETA(DisplayName = "max")
};

UENUM(BlueprintType)
enum class EHoudini_PackedPrimInstancingMode : uint8
{
    Houdini_PACKEDPRIM_INSTANCING_MODE_INVALID UMETA(DisplayName = "INVALID"),
    Houdini_PACKEDPRIM_INSTANCING_MODE_DISABLED UMETA(DisplayName = "DISABLED"),
    Houdini_PACKEDPRIM_INSTANCING_MODE_HIERARCHY UMETA(DisplayName = "HIERARCHY"),
    Houdini_PACKEDPRIM_INSTANCING_MODE_FLAT UMETA(DisplayName = "FLAT"),
    Houdini_PACKEDPRIM_INSTANCING_MODE_MAX UMETA(DisplayName = "MAX")
};

/// //////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
class FEnumParser
{
    TMap<FString, T> EnumMap;
public:
    FEnumParser() {};
    T ParsarEnum(const FString& value)
    {
        T* valuePtr = EnumMap.Find(value);
        check(valuePtr);
        return *valuePtr;
    }
    FString FindKey(const T value)
    {
        const FString* str = EnumMap.FindKey(value);
        check(str);
        return *str;
    }
};

FEnumParser<HAPI_NodeFlags>::FEnumParser()
{
    EnumMap.Emplace("Houdini_NODEFLAGS_ANY", HAPI_NODEFLAGS_ANY);
    EnumMap.Emplace("Houdini_NODEFLAGS_NONE", HAPI_NODEFLAGS_NONE);
    EnumMap.Emplace("Houdini_NODEFLAGS_DISPLAY", HAPI_NODEFLAGS_DISPLAY);
    EnumMap.Emplace("Houdini_NODEFLAGS_RENDER", HAPI_NODEFLAGS_RENDER);
    EnumMap.Emplace("Houdini_NODEFLAGS_TEMPLATED", HAPI_NODEFLAGS_TEMPLATED);
    EnumMap.Emplace("Houdini_NODEFLAGS_LOCKED", HAPI_NODEFLAGS_LOCKED);
    EnumMap.Emplace("Houdini_NODEFLAGS_EDITABLE", HAPI_NODEFLAGS_EDITABLE);
    EnumMap.Emplace("Houdini_NODEFLAGS_BYPASS", HAPI_NODEFLAGS_BYPASS);
    EnumMap.Emplace("Houdini_NODEFLAGS_NETWORK", HAPI_NODEFLAGS_NETWORK);
    EnumMap.Emplace("Houdini_NODEFLAGS_OBJ_GEOMETRY", HAPI_NODEFLAGS_OBJ_GEOMETRY);
    EnumMap.Emplace("Houdini_NODEFLAGS_OBJ_CAMERA", HAPI_NODEFLAGS_OBJ_CAMERA);
    EnumMap.Emplace("Houdini_NODEFLAGS_OBJ_LIGHT", HAPI_NODEFLAGS_OBJ_LIGHT);
    EnumMap.Emplace("Houdini_NODEFLAGS_OBJ_SUBNET", HAPI_NODEFLAGS_OBJ_SUBNET);
    EnumMap.Emplace("Houdini_NODEFLAGS_SOP_CURVE", HAPI_NODEFLAGS_SOP_CURVE);
    EnumMap.Emplace("Houdini_NODEFLAGS_SOP_GUIDE", HAPI_NODEFLAGS_SOP_GUIDE);
    EnumMap.Emplace("Houdini_NODEFLAGS_TOP_NONSCHEDULER", HAPI_NODEFLAGS_TOP_NONSCHEDULER);
}
FEnumParser<HAPI_NodeType>::FEnumParser()
{
    EnumMap.Emplace("Houdini_NODETYPE_ANY", HAPI_NODETYPE_ANY);
    EnumMap.Emplace("Houdini_NODETYPE_NONE", HAPI_NODETYPE_NONE);
    EnumMap.Emplace("Houdini_NODETYPE_OBJ", HAPI_NODETYPE_OBJ);
    EnumMap.Emplace("Houdini_NODETYPE_SOP", HAPI_NODETYPE_SOP);
    EnumMap.Emplace("Houdini_NODETYPE_CHOP", HAPI_NODETYPE_CHOP);
    EnumMap.Emplace("Houdini_NODETYPE_ROP", HAPI_NODETYPE_ROP);
    EnumMap.Emplace("Houdini_NODETYPE_SHOP", HAPI_NODETYPE_SHOP);
    EnumMap.Emplace("Houdini_NODETYPE_COP", HAPI_NODETYPE_COP);
    EnumMap.Emplace("Houdini_NODETYPE_VOP", HAPI_NODETYPE_VOP);
    EnumMap.Emplace("Houdini_NODETYPE_DOP", HAPI_NODETYPE_DOP);
    EnumMap.Emplace("Houdini_NODETYPE_TOP", HAPI_NODETYPE_TOP);
}
FEnumParser<HAPI_PartType>::FEnumParser()
{
    EnumMap.Emplace("Houdini_PARTTYPE_INVALID", HAPI_PARTTYPE_INVALID);
    EnumMap.Emplace("Houdini_PARTTYPE_MESH", HAPI_PARTTYPE_MESH);
    EnumMap.Emplace("Houdini_PARTTYPE_CURVE", HAPI_PARTTYPE_CURVE);
    EnumMap.Emplace("Houdini_PARTTYPE_VOLUME", HAPI_PARTTYPE_VOLUME);
    EnumMap.Emplace("Houdini_PARTTYPE_INSTANCER", HAPI_PARTTYPE_INSTANCER);
    EnumMap.Emplace("Houdini_PARTTYPE_BOX", HAPI_PARTTYPE_BOX);
    EnumMap.Emplace("Houdini_PARTTYPE_SPHERE", HAPI_PARTTYPE_SPHERE);
    EnumMap.Emplace("Houdini_PARTTYPE_MAX", HAPI_PARTTYPE_MAX);
}

FEnumParser<HAPI_AttributeOwner>::FEnumParser()
{
    EnumMap.Emplace("Houdini_ATTROWNER_INVALID", HAPI_ATTROWNER_INVALID);
    EnumMap.Emplace("Houdini_ATTROWNER_VERTEX", HAPI_ATTROWNER_VERTEX);
    EnumMap.Emplace("Houdini_ATTROWNER_POINT", HAPI_ATTROWNER_POINT);
    EnumMap.Emplace("Houdini_ATTROWNER_PRIM", HAPI_ATTROWNER_PRIM);
    EnumMap.Emplace("Houdini_ATTROWNER_DETAIL", HAPI_ATTROWNER_DETAIL);
    EnumMap.Emplace("Houdini_ATTROWNER_MAX", HAPI_ATTROWNER_MAX);
}

FEnumParser<HAPI_StorageType>::FEnumParser()
{
    EnumMap.Emplace("Houdini_STORAGETYPE_INVALID", HAPI_STORAGETYPE_INVALID);
    EnumMap.Emplace("Houdini_STORAGETYPE_INT", HAPI_STORAGETYPE_INT);
    EnumMap.Emplace("Houdini_STORAGETYPE_INT64", HAPI_STORAGETYPE_INT64);
    EnumMap.Emplace("Houdini_STORAGETYPE_FLOAT", HAPI_STORAGETYPE_FLOAT);
    EnumMap.Emplace("Houdini_STORAGETYPE_FLOAT64", HAPI_STORAGETYPE_FLOAT64);
    EnumMap.Emplace("Houdini_STORAGETYPE_STRING", HAPI_STORAGETYPE_STRING);
    EnumMap.Emplace("Houdini_STORAGETYPE_MAX", HAPI_STORAGETYPE_MAX);
}

FEnumParser<HAPI_CurveType>::FEnumParser()
{
    EnumMap.Emplace("Houdini_CURVETYPE_INVALID", HAPI_CURVETYPE_INVALID);
    EnumMap.Emplace("Houdini_CURVETYPE_LINEAR", HAPI_CURVETYPE_LINEAR);
    EnumMap.Emplace("Houdini_CURVETYPE_NURBS", HAPI_CURVETYPE_NURBS);
    EnumMap.Emplace("Houdini_CURVETYPE_BEZIER", HAPI_CURVETYPE_BEZIER);
    EnumMap.Emplace("Houdini_CURVETYPE_MAX", HAPI_CURVETYPE_MAX);
}

FEnumParser<HAPI_RampType>::FEnumParser()
{
    EnumMap.Emplace("Houdini_RampType_INVALID", HAPI_RAMPTYPE_INVALID);
    EnumMap.Emplace("Houdini_RampType_FLOAT", HAPI_RAMPTYPE_FLOAT);
    EnumMap.Emplace("Houdini_RampType_COLOR", HAPI_RAMPTYPE_COLOR);
    EnumMap.Emplace("Houdini_RampType_MAX", HAPI_RAMPTYPE_MAX);
}

FEnumParser<HAPI_PackedPrimInstancingMode>::FEnumParser()
{
    EnumMap.Emplace("Houdini_PACKEDPRIM_INSTANCING_MODE_INVALID", HAPI_PACKEDPRIM_INSTANCING_MODE_INVALID);
    EnumMap.Emplace("Houdini_PACKEDPRIM_INSTANCING_MODE_DISABLED", HAPI_PACKEDPRIM_INSTANCING_MODE_DISABLED);
    EnumMap.Emplace("Houdini_PACKEDPRIM_INSTANCING_MODE_HIERARCHY", HAPI_PACKEDPRIM_INSTANCING_MODE_HIERARCHY);
    EnumMap.Emplace("Houdini_PACKEDPRIM_INSTANCING_MODE_FLAT", HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT);
    EnumMap.Emplace("Houdini_PACKEDPRIM_INSTANCING_MODE_MAX", HAPI_PACKEDPRIM_INSTANCING_MODE_MAX);
}

///////////////////////////////////////////////Struct////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FHoudiniSession
{
    GENERATED_BODY();
    /// The type of session determines the which implementation will be
    /// used to communicate with the Houdini Engine library.
    UPROPERTY(BlueprintReadWrite,VisibleAnywhere,category="zjhHoudiniUnrealPlugin")
        EHoudini_SessionType HSessionType;

    /// Some session types support multiple simultaneous sessions. This means
    /// that each session needs to have a unique identifier.
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, category = "zjhHoudiniUnrealPlugin")
        int64 HSessionId;

    //??????
    FHoudiniSession()
        :HSessionType(EHoudini_SessionType::Houdini_SESSION_INPROCESS),
        HSessionId(0)
    {}

public:
    HAPI_Session ToHAPI_Session()
    {
        HAPI_Session a_session;
        a_session.id = (HAPI_SessionId)HSessionId;
        a_session.type = (HAPI_SessionType)HSessionType;
        return a_session;
    }
    static FHoudiniSession FromHAPI_Session(const HAPI_Session& in_HAPISession )
    {
        FHoudiniSession a_fsession;
        a_fsession.HSessionId = (int64)in_HAPISession.id;
        a_fsession.HSessionType = (EHoudini_SessionType)in_HAPISession.type;
        return a_fsession;
    }
};

USTRUCT(BlueprintType)
struct FHoudiniCookOption
{
    GENERATED_USTRUCT_BODY();

    HAPI_CookOptions houdiniCooOption;
};

USTRUCT(BlueprintType)
struct FHoudiniPartInfo
{
    GENERATED_USTRUCT_BODY();

    HAPI_PartInfo houPartInfo;
};

USTRUCT(BlueprintType)
struct FHoudiniAtrributeInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_AttributeInfo houAttributeInfo;
};

USTRUCT(BlueprintType)
struct FVertexList
{
    GENERATED_USTRUCT_BODY();
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, category = "zjhAddAttrs | common")
        TArray<int> vertexList;
};

USTRUCT(BlueprintType)
struct FHoudiniNodeInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_NodeInfo houNodeInfo;
};

USTRUCT(BlueprintType)
struct FHoudiniTranform
{
    GENERATED_USTRUCT_BODY();
    HAPI_Transform houTransform;
};

USTRUCT(BlueprintType)
struct FHoudiniTranformEuler
{
    GENERATED_USTRUCT_BODY();
    HAPI_TransformEuler houTransformEuler;
};

USTRUCT(BlueprintType)
struct FHoudiniObjectInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_ObjectInfo  houObjectInfo;
};

USTRUCT(BlueprintType)
struct FHoudiniGeoInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_GeoInfo   houGeoInfo;
};

USTRUCT(BlueprintType)
struct FHoudiniAssetInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_AssetInfo   houAssetInfo;
};

USTRUCT(BlueprintType)
struct FHoudiniCurveInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_CurveInfo   houCurvetInfo;
};

USTRUCT(BlueprintType)
struct FHoudiniParamInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_ParmInfo   houParmInfo;
};

USTRUCT(BlueprintType)
struct FHIntVector4D
{
    GENERATED_USTRUCT_BODY();
    UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Houdini Variables")
        int32 X;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Houdini Variables")
        int32 Y;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Houdini Variables")
        int32 Z;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Houdini Variables")
        int32 W;
    FHIntVector4D() :X(0),Y(0),Z(0),W(0) {}
    FHIntVector4D(int32 inx,int32 iny,int32 inz,int32 inw):X(inx),Y(iny),Z(inz),W(inw){}
};

USTRUCT(BlueprintType)
struct FHoudiniParmChoiceInfo
{
    GENERATED_USTRUCT_BODY();
    HAPI_ParmChoiceInfo    houParmChoiceInfo;
};

USTRUCT(BlueprintType)
struct FUnrealSplitedTransforms
{
    GENERATED_USTRUCT_BODY();
    UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Houdini Variables")
        TArray<FTransform> transformList;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Houdini Variables")
        FString partName;
};

