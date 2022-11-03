#pragma once
#include "CoreMinimal.h"
#include "HAPI/HAPI.h"
#include "HAPIToUnrealCommonBridge.generated.h"

UENUM()
enum class EHoudini_SessionType
{
    Houdini_SESSION_INPROCESS,
    Houdini_SESSION_THRIFT,
    Houdini_SESSION_CUSTOM1,
    Houdini_SESSION_CUSTOM2,
    Houdini_SESSION_CUSTOM3,
    Houdini_SESSION_MAX
};

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