#pragma once
#include "CoreMinimal.h"
#include "HAPI/HAPI.h"
#include "HAPIToUnrealCommonBridge.generated.h"

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

    //≥ı ºªØ
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