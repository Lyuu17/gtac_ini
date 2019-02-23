#include "StdInc.h"

#include <stdio.h>

#include <GalacticC.h>
#include <ini.h>

GReflectedClass* g_pINIFileClass;

extern "C" __declspec(dllexport) const char* MODULE_NAME = "INI";

extern "C" __declspec(dllexport) void* _cdecl RegisterModule(GReflectedNamespace* pNamespace, void* pReserved) {
	g_pINIFileClass = Galactic_ReflectedNamespace_NewClass(pNamespace, "INIFile");

	Galactic_ReflectedNamespace_RegisterFunction(pNamespace, "Create", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		CSimpleIni *ini = new CSimpleIni();

		GRefCounted* pRef = Galactic_Referenceable_Create(g_pINIFileClass);
		Galactic_Referenceable_SetPrivate(pRef, (void*)ini);
		Galactic_NativeState_ReturnReferenceable(pState, pRef);
		Galactic_RefCounted_Release(pRef);

		return true;
	});

	Galactic_ReflectedNamespace_RegisterFunction(pNamespace, "Destroy", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_CheckReferenceable(pState, 0);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.Destroy: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.Destroy: missing pointer\n");
		}

		delete ini;

		ini = NULL;

		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "Errno", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		int err = 0;

		if (!Galactic_NativeState_CheckInt32(pState, 0, err)) {
			return Galactic_NativeState_SetError(pState, "INI.Errno: missing errno\n");
		}

		switch (err) {
		case SI_OK:
			Galactic_NativeState_ReturnString(pState, "ok", 2);
			break;
		case SI_UPDATED:
			Galactic_NativeState_ReturnString(pState, "updated", 7);
			break;
		case SI_INSERTED:
			Galactic_NativeState_ReturnString(pState, "inserted", 8);
			break;
		case SI_FAIL:
			Galactic_NativeState_ReturnString(pState, "fail", 4);
			break;
		case SI_NOMEM:
			Galactic_NativeState_ReturnString(pState, "nomem", 5);
			break;
		case SI_FILE:
			//todo: probably extend this?
			Galactic_NativeState_ReturnString(pState, "file", 4);
			break;
		}

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "Reset", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.GetString: missing pointer\n");
		}

		const char *szDefaultValue = Galactic_NativeState_CheckString(pState, 2);
		if (!szDefaultValue) szDefaultValue = "";

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.GetString: missing pointer\n");
		}

		ini->Reset();

		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});


	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "Delete", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.Delete: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.Delete: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.Delete: empty key name\n");
		}

		bool bRemoveEmpty = true;
		if (!Galactic_NativeState_CheckBoolean(pState, 2, bRemoveEmpty)) bRemoveEmpty = true;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.Delete: missing pointer\n");
		}

		bool result = ini->Delete(szSection, szKey, bRemoveEmpty);
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "IsEmpty", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.IsEmpty: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.IsEmpty: missing pointer\n");
		}

		bool result = ini->IsEmpty();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "IsMultiKey", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.IsMultiKey: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.IsMultiKey: missing pointer\n");
		}

		bool result = ini->IsMultiKey();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "IsMultiLine", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.IsMultiLine: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.IsMultiLine: missing pointer\n");
		}

		bool result = ini->IsMultiLine();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "IsUnicode", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.IsUnicode: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.IsUnicode: missing pointer\n");
		}

		bool result = ini->IsUnicode();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "IsUsingSpaces", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.IsUsingSpaces: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.IsUsingSpaces: missing pointer\n");
		}

		bool result = ini->UsingSpaces();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "SetUnicode", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.SetUnicode: missing pointer\n");
		}

		bool bUnicode = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 0, bUnicode)) {
			return Galactic_NativeState_SetError(pState, "INI.SetUnicode: empty boolean value\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.SetUnicode: missing pointer\n");
		}

		ini->SetUnicode(bUnicode);

		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "LoadFile", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.LoadFile: missing pointer\n");
		}

		const char *szFilename = Galactic_NativeState_CheckString(pState, 0);
		if (!szFilename) {
			return Galactic_NativeState_SetError(pState, "INI.LoadFile: empty filename\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		
		SI_Error result = ini->LoadFile(szFilename);
		Galactic_NativeState_ReturnInt32(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "SaveFile", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.SaveFile: missing pointer\n");
		}

		const char *szFilename = Galactic_NativeState_CheckString(pState, 0);
		if (!szFilename) {
			return Galactic_NativeState_SetError(pState, "INI.SaveFile: empty filename\n");
		}

		bool bSign = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 1, bSign)) {
			bSign = false;
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (ini->SaveFile(szFilename, true) < 0) {
			return Galactic_NativeState_SetError(pState, "INI.SaveFile: error saving file %s\n", szFilename);
		}

		if (ini) delete ini;
		ini = NULL;

		Galactic_Referenceable_SetPrivate(pINIRef, nullptr);
		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "GetBoolValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.GetBoolValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.GetBoolValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.GetBoolValue: empty key name\n");
		}

		bool bDefaultValue = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 2, bDefaultValue)) bDefaultValue = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.GetBoolValue: missing pointer\n");
		}
		
		bool result = ini->GetBoolValue(szSection, szKey, bDefaultValue);

		Galactic_NativeState_ReturnBoolean(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "GetIntValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.GetIntValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.GetIntValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.GetIntValue: empty key name\n");
		}

		int64_t nDefaultValue = 0;
		if (!Galactic_NativeState_CheckInt64(pState, 2, nDefaultValue)) nDefaultValue = 0;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.GetIntValue: missing pointer\n");
		}

		int64_t result = ini->GetLongValue(szSection, szKey, nDefaultValue);

		Galactic_NativeState_ReturnInt64(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "GetFloatValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.GetFloatValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.GetFloatValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.GetFloatValue: empty key name\n");
		}

		double nDefaultValue = 0.0;
		if (!Galactic_NativeState_CheckDouble(pState, 2, nDefaultValue)) nDefaultValue = 0.0;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.GetFloatValue: missing pointer\n");
		}

		double result = ini->GetDoubleValue(szSection, szKey, nDefaultValue);

		Galactic_NativeState_ReturnDouble(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "GetString", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.GetString: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.GetString: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.GetString: empty key name\n");
		}

		const char *szDefaultValue = Galactic_NativeState_CheckString(pState, 2);
		if (!szDefaultValue) szDefaultValue = "";

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.GetString: missing pointer\n");
		}

		const char *result = ini->GetValue(szSection, szKey, szDefaultValue);

		Galactic_NativeState_ReturnString(pState, result, strlen(result));

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "SetBoolValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.SetBoolValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.SetBoolValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.SetBoolValue: empty key name\n");
		}

		bool bValue = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 2, bValue)) {
			return Galactic_NativeState_SetError(pState, "INI.SetBoolValue: empty boolean value\n");
		}

		const char *szComment = Galactic_NativeState_CheckString(pState, 3);
		if (!szComment) szComment = 0;

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.SetBoolValue: missing pointer\n");
		}

		SI_Error result = ini->SetBoolValue(szSection, szKey, bValue, szComment, bForceReplace);
		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "SetIntValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.SetIntValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.SetIntValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.SetIntValue: empty key name\n");
		}

		int64_t iValue = 0;
		if (!Galactic_NativeState_CheckInt64(pState, 2, iValue)) {
			return Galactic_NativeState_SetError(pState, "INI.SetIntValue: empty integer value\n");
		}

		const char *szComment = Galactic_NativeState_CheckString(pState, 3);
		if (!szComment) szComment = 0;

		bool bUseHex = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bUseHex)) bUseHex = false;

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 5, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.SetIntValue: missing pointer\n");
		}

		SI_Error result = ini->SetLongValue(szSection, szKey, iValue, szComment, bUseHex, bForceReplace);
		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "SetFloatValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.SetFloatValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.SetFloatValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.SetFloatValue: empty key name\n");
		}

		double nValue = 0.0;
		if (!Galactic_NativeState_CheckDouble(pState, 2, nValue)) {
			return Galactic_NativeState_SetError(pState, "INI.SetFloatValue: empty float value\n");
		}

		const char *szComment = Galactic_NativeState_CheckString(pState, 3);
		if (!szComment) szComment = 0;

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.SetFloatValue: missing pointer\n");
		}

		SI_Error result = ini->SetDoubleValue(szSection, szKey, nValue, szComment, bForceReplace);
		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "SetString", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "INI.SetString: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "INI.SetString: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "INI.SetString: empty key name\n");
		}

		const char *szValue = Galactic_NativeState_CheckString(pState, 2);
		if (!szValue) {
			return Galactic_NativeState_SetError(pState, "INI.SetString: empty string value\n");
		}

		const char *szComment = Galactic_NativeState_CheckString(pState, 3);
		if (!szComment) szComment = 0;

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "INI.SetString: missing pointer\n");
		}

		SI_Error result = ini->SetValue(szSection, szKey, szValue, szComment, bForceReplace);
		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	printf("[INI] Module loaded \n");
	printf("[INI] Build date: %s \n", __DATE__);

	return nullptr;
}

extern "C" __declspec(dllexport) void _cdecl UnregisterModule(void* pUser)
{
}