#pragma once

#if defined(ENGINE_STATIC) && !defined(ENGINE_DLL)
#define GALACTICC_API
#else
#ifdef GALACTICC_BUILDING
#define GALACTICC_API GALACTIC_DLLEXPORT
#else
#define GALACTICC_API
#endif
#endif

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GALACTICC_BUILDING
typedef Galactic3D::CBase GObject;
typedef Galactic3D::CRefCounted GRefCounted;
typedef Galactic3D::CNativeState GNativeState;
typedef Galactic3D::ReflectedClass GReflectedClass;
typedef Galactic3D::ReflectedNamespace GReflectedNamespace;
typedef Galactic3D::CScripting GScripting;
#else
#ifdef __cplusplus
class GObject;
class GRefCounted;
class GNativeState;
class GReflectedClass;
class GReflectedNamespace;
class GScripting;
#else
typedef void GObject;
typedef void GRefCounted;
typedef void GNativeState;
typedef void GReflectedClass;
typedef void GReflectedNamespace;
typedef void GScripting;
#endif
#endif

typedef void(*GDestructorProc)(GRefCounted*);
typedef bool(*GScriptFunctionCallback) (GNativeState* pState, int32_t argc, void* pUser);

enum eGArgumentType
{
	GARGUMENT_NIL,
	GARGUMENT_BOOLEAN,
	GARGUMENT_STRING,
	GARGUMENT_TABLE,//NEVER USED
	GARGUMENT_INTEGER,
	GARGUMENT_FLOAT,
	GARGUMENT_ELEMENT,//NETWORK ONLY!
	GARGUMENT_REFERENCEABLE,
	GARGUMENT_VECTOR2D,
	GARGUMENT_VECTOR3D,
	GARGUMENT_VECTOR4D,
	GARGUMENT_MATRIX4X4,
	GARGUMENT_UNDEFINED,
	GARGUMENT_LUAREF,//SPECIAL; GETS SWALLOWED!
	GARGUMENT_JSVALUE,//SPECIAL; GETS SWALLOWED!
	GARGUMENT_LONGINTEGER,//64-bit
	GARGUMENT_DOUBLE,//64-bit
	GARGUMENT_ARRAY,
	GARGUMENT_MAP,//NEVER USED BUT RESERVED
	GARGUMENT_FUNCTION,//GETS SWALLOWED!
	GARGUMENT_SQREF,//SPECIAL; GETS SWALLOWED!
};

extern GALACTICC_API size_t Galactic_RefCounted_GetRefCount(GRefCounted* pObject);
extern GALACTICC_API size_t Galactic_RefCounted_AddRef(GRefCounted* pObject);
extern GALACTICC_API size_t Galactic_RefCounted_Release(GRefCounted* pObject);

extern GALACTICC_API GRefCounted* Galactic_Referenceable_Create(GReflectedClass* pClass, GDestructorProc Destructor = nullptr);
extern GALACTICC_API void Galactic_Referenceable_SetPrivate(GRefCounted* pObject, void* pPrivate);
extern GALACTICC_API void* Galactic_Referenceable_GetPrivate(GRefCounted* pObject);
extern GALACTICC_API bool Galactic_Referenceable_IsA(GRefCounted* pObject, GReflectedClass* pClass);

extern GALACTICC_API GObject* Galactic_Argument_Create(eGArgumentType Type);
extern GALACTICC_API GObject* Galactic_Argument_Clone(const GObject* pArgument);
extern GALACTICC_API unsigned char Galactic_Argument_GetType(const GObject* pArgument);
extern GALACTICC_API const char* Galactic_Argument_GetTypeName(const GObject* pArgument);

extern GALACTICC_API bool Galactic_Argument_IsNull(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsBoolean(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsString(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsReferenceable(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsVector2D(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsVector3D(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsVector4D(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsMatrix4x4(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsNumber(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsArray(const GObject* pArgument);
extern GALACTICC_API bool Galactic_Argument_IsFunction(const GObject* pArgument);

// !! These functions don't check the type is correct before converting !!
extern GALACTICC_API bool Galactic_Argument_ToBoolean(const GObject* pArgument);
extern GALACTICC_API const char* Galactic_Argument_ToString(const GObject* pArgument, size_t* pLength);
extern GALACTICC_API GRefCounted* Galactic_Argument_ToReferenceable(const GObject* pArgument, GReflectedClass* pClass = nullptr);
extern GALACTICC_API void Galactic_Argument_ToVector2D(const GObject* pArgument, float& x, float& y);
extern GALACTICC_API void Galactic_Argument_ToVector3D(const GObject* pArgument, float& x, float& y, float& z);
extern GALACTICC_API void Galactic_Argument_ToVector4D(const GObject* pArgument, float& x, float& y, float& z, float& w);
extern GALACTICC_API void Galactic_Argument_ToMatrix4x4(const GObject* pArgument, float* prgfRowMajorEntries);
extern GALACTICC_API void Galactic_Argument_ToInt32(const GObject* pArgument, int32_t& Value);
extern GALACTICC_API void Galactic_Argument_ToUInt32(const GObject* pArgument, uint32_t& Value);
extern GALACTICC_API void Galactic_Argument_ToInt64(const GObject* pArgument, int64_t& Value);
extern GALACTICC_API void Galactic_Argument_ToUInt64(const GObject* pArgument, uint64_t& Value);
extern GALACTICC_API void Galactic_Argument_ToSingle(const GObject* pArgument, float& Value);
extern GALACTICC_API void Galactic_Argument_ToDouble(const GObject* pArgument, double& Value);
extern GALACTICC_API GRefCounted* Galactic_Argument_ToFunction(const GObject* pArgument); // Doesn't AddRef

extern GALACTICC_API void Galactic_Argument_SetBoolean(GObject* pArgument, bool b);
extern GALACTICC_API void Galactic_Argument_SetString(GObject* pArgument, const char* psz, size_t Length);
extern GALACTICC_API void Galactic_Argument_SetStringZ(GObject* pArgument, const char* psz);
extern GALACTICC_API void Galactic_Argument_SetVector2D(GObject* pArgument, float x, float y);
extern GALACTICC_API void Galactic_Argument_SetVector3D(GObject* pArgument, float x, float y, float z);
extern GALACTICC_API void Galactic_Argument_SetMatrix4x4(GObject* pArgument, const float* prgfRowMajorEntries);
extern GALACTICC_API void Galactic_Argument_SetInt32(GObject* pArgument, int32_t Value);
extern GALACTICC_API void Galactic_Argument_SetUInt32(GObject* pArgument, uint32_t Value);
extern GALACTICC_API void Galactic_Argument_SetInt64(GObject* pArgument, int64_t Value);
extern GALACTICC_API void Galactic_Argument_SetUInt64(GObject* pArgument, uint64_t Value);
extern GALACTICC_API void Galactic_Argument_SetSingle(GObject* pArgument, float Value);
extern GALACTICC_API void Galactic_Argument_SetDouble(GObject* pArgument, double Value);

extern GALACTICC_API size_t Galactic_Array_GetCount(const GObject* pArray);
extern GALACTICC_API const GObject* Galactic_Array_GetAt(const GObject* pArray, size_t Index);
extern GALACTICC_API void Galactic_Array_Clear(GObject* pArray);
extern GALACTICC_API void Galactic_Array_Resize(GObject* pArray, size_t Size); // Will clear
extern GALACTICC_API void Galactic_Array_Insert(GObject* pArray, const GObject* pArgument);
extern GALACTICC_API void Galactic_Array_InsertAndOwn(GObject* pArray, GObject* pArgument); // Don't delete pArgument!
extern GALACTICC_API void Galactic_Array_Move(GObject* pArray, GObject* pTargetArray);

extern GALACTICC_API void Galactic_Map_Clear(GObject* pMap);
extern GALACTICC_API void Galactic_Map_Resize(GObject* pMap, size_t Size); // Will clear
extern GALACTICC_API const GObject* Galactic_Map_Get(const GObject* pMap, const char* pszKey);
extern GALACTICC_API void Galactic_Map_Set(GObject* pMap, const char* pszKey, const GObject* pArgument);
extern GALACTICC_API void Galactic_Map_SetAndOwn(GObject* pMap, const char* pszKey, GObject* pArgument); // Don't delete pArgument!

extern GALACTICC_API bool Galactic_NativeState_SetError(GNativeState* pNativeState, const char* pszFormat, ...);
extern GALACTICC_API bool Galactic_NativeState_SetErrorV(GNativeState* pNativeState, const char* pszFormat, va_list args);
extern GALACTICC_API const GObject* Galactic_NativeState_GetArgument(GNativeState* pNativeState, size_t Index);
extern GALACTICC_API bool Galactic_NativeState_CheckBoolean(GNativeState* pNativeState, size_t Index, bool& bValue);
extern GALACTICC_API bool Galactic_NativeState_CheckInt32(GNativeState* pNativeState, size_t Index, int32_t& Value);
extern GALACTICC_API bool Galactic_NativeState_CheckUInt32(GNativeState* pNativeState, size_t Index, uint32_t& Value);
extern GALACTICC_API bool Galactic_NativeState_CheckInt64(GNativeState* pNativeState, size_t Index, int64_t& Value);
extern GALACTICC_API bool Galactic_NativeState_CheckUInt64(GNativeState* pNativeState, size_t Index, uint64_t& Value);
extern GALACTICC_API bool Galactic_NativeState_CheckSingle(GNativeState* pNativeState, size_t Index, float& Value);
extern GALACTICC_API bool Galactic_NativeState_CheckDouble(GNativeState* pNativeState, size_t Index, double& Value);
extern GALACTICC_API const char* Galactic_NativeState_CheckString(GNativeState* pNativeState, size_t Index, size_t* pLength = nullptr);
extern GALACTICC_API bool Galactic_NativeState_CheckVector2D(GNativeState* pNativeState, size_t Index, float& x, float& y);
extern GALACTICC_API bool Galactic_NativeState_CheckVector3D(GNativeState* pNativeState, size_t Index, float& x, float& y, float& z);
extern GALACTICC_API bool Galactic_NativeState_CheckMatrix4x4(GNativeState* pNativeState, size_t Index, float* prgfRowMajorEntries);
extern GALACTICC_API bool Galactic_NativeState_CheckClass(GNativeState* pNativeState, GReflectedClass* pClass, size_t Index, bool bNull, GRefCounted*& pReferenceable);
extern GALACTICC_API GRefCounted* Galactic_NativeState_CheckReferenceable(GNativeState* pNativeState, size_t Index);
extern GALACTICC_API GRefCounted* Galactic_NativeState_CheckFunction(GNativeState* pNativeState, size_t Index);
extern GALACTICC_API GRefCounted* Galactic_NativeState_CheckStream(GNativeState* pNativeState, size_t Index);
extern GALACTICC_API GRefCounted* Galactic_NativeState_GetThis(GNativeState* pNativeState, GReflectedClass* pClass);

extern GALACTICC_API void Galactic_NativeState_Return(GNativeState* pNativeState, const GObject* pArgument);
extern GALACTICC_API void Galactic_NativeState_ReturnAndOwn(GNativeState* pNativeState, GObject* pArgument);
extern GALACTICC_API void Galactic_NativeState_ReturnNull(GNativeState* pNativeState);
extern GALACTICC_API void Galactic_NativeState_ReturnUndefined(GNativeState* pNativeState);
extern GALACTICC_API void Galactic_NativeState_ReturnBoolean(GNativeState* pNativeState, bool b);
extern GALACTICC_API void Galactic_NativeState_ReturnString(GNativeState* pNativeState, const char* psz, size_t Length);
extern GALACTICC_API void Galactic_NativeState_ReturnStringZ(GNativeState* pNativeState, const char* psz);
extern GALACTICC_API void Galactic_NativeState_ReturnInt32(GNativeState* pNativeState, int32_t Value);
extern GALACTICC_API void Galactic_NativeState_ReturnUInt32(GNativeState* pNativeState, uint32_t Value);
extern GALACTICC_API void Galactic_NativeState_ReturnInt64(GNativeState* pNativeState, int64_t Value);
extern GALACTICC_API void Galactic_NativeState_ReturnUInt64(GNativeState* pNativeState, uint64_t Value);
extern GALACTICC_API void Galactic_NativeState_ReturnSingle(GNativeState* pNativeState, float Value);
extern GALACTICC_API void Galactic_NativeState_ReturnDouble(GNativeState* pNativeState, double Value);
extern GALACTICC_API void Galactic_NativeState_ReturnVector2D(GNativeState* pNativeState, float x, float y);
extern GALACTICC_API void Galactic_NativeState_ReturnVector3D(GNativeState* pNativeState, float x, float y, float z);
extern GALACTICC_API void Galactic_NativeState_ReturnMatrix4x4(GNativeState* pNativeState, const float* prgfRowMajorEntries);
extern GALACTICC_API void Galactic_NativeState_ReturnReferenceable(GNativeState* pNativeState, GRefCounted* pReferenceable);
extern GALACTICC_API void Galactic_NativeState_ReturnStream(GNativeState* pNativeState, GRefCounted* pStream);

// Note: pszName should remain allocated for lifetime of this class
extern GALACTICC_API void Galactic_ReflectedNamespace_Clear(GReflectedNamespace* pNamespace);
extern GALACTICC_API GReflectedNamespace* Galactic_ReflectedNamespace_GetNamespace(GReflectedNamespace* pNamespace, const char* pszName);
extern GALACTICC_API GReflectedNamespace* Galactic_ReflectedNamespace_AddNamespace(GReflectedNamespace* pNamespace, const char* pszName);
extern GALACTICC_API bool Galactic_ReflectedNamespace_RegisterFunction(GReflectedNamespace* pNamespace, const char* pszName, const char* pszMask, GScriptFunctionCallback Callback, void* pUser = nullptr);
extern GALACTICC_API bool Galactic_ReflectedNamespace_AddProperty(GReflectedNamespace* pNamespace, void* pUser, const char* pszName, unsigned char Type, GScriptFunctionCallback pGetter, GScriptFunctionCallback pSetter = nullptr);
extern GALACTICC_API GReflectedClass* Galactic_ReflectedNamespace_NewClass(GReflectedNamespace* pNamespace, const char* pszName, GReflectedClass* pParent = nullptr);

extern GALACTICC_API const char* Galactic_ReflectedClass_GetName(GReflectedClass* pClass);
extern GALACTICC_API void Galactic_ReflectedClass_Clear(GReflectedClass* pClass);
extern GALACTICC_API bool Galactic_ReflectedClass_RegisterConstructor(GReflectedClass* pClass, const char* pszMask, GScriptFunctionCallback pFunction, void* pUser = nullptr);
extern GALACTICC_API bool Galactic_ReflectedClass_RegisterClone(GReflectedClass* pClass, GScriptFunctionCallback pFunction, void* pUser = nullptr);
extern GALACTICC_API bool Galactic_ReflectedClass_RegisterMove(GReflectedClass* pClass, GScriptFunctionCallback pFunction, void* pUser = nullptr);
extern GALACTICC_API bool Galactic_ReflectedClass_RegisterToString(GReflectedClass* pClass, GScriptFunctionCallback pFunction, void* pUser = nullptr);
extern GALACTICC_API bool Galactic_ReflectedClass_RegisterFunction(GReflectedClass* pClass, const char* pszName, const char* pszMask, GScriptFunctionCallback pFunction, void* pUser = nullptr);
extern GALACTICC_API bool Galactic_ReflectedClass_AddProperty(GReflectedClass* pClass, void* pUser, const char* pszName, unsigned char Type, GScriptFunctionCallback pGetter, GScriptFunctionCallback pSetter = nullptr);
extern GALACTICC_API bool Galactic_ReflectedClass_IsA(GReflectedClass* pClass, GReflectedClass* pOtherClass);

extern GALACTICC_API GReflectedNamespace* Galactic_Scripting_GetGlobal(GScripting* pScripting);
extern GALACTICC_API void Galactic_Scripting_CollectGarbage(GScripting* pScripting);

extern GALACTICC_API size_t Galactic_Stream_Read(GRefCounted* pStream, void* destinationBuffer, size_t size);
extern GALACTICC_API size_t Galactic_Stream_Write(GRefCounted* pStream, const void* data, size_t size);
extern GALACTICC_API bool Galactic_Stream_Seek(GRefCounted* pStream, int64_t Offset, int32_t Origin);
extern GALACTICC_API uint64_t Galactic_Stream_Tell(GRefCounted* pStream);
extern GALACTICC_API uint64_t Galactic_Stream_GetLength(GRefCounted* pStream);
extern GALACTICC_API bool Galactic_Stream_SetModifiedTime(GRefCounted* pStream, time_t Time);

extern GALACTICC_API GRefCounted* Galactic_CreateMemoryStream(void* data, size_t length, bool bReadable, bool bWritable);

#ifdef __cplusplus
}
#endif
