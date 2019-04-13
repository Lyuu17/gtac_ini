#include "StdInc.h"

#include <stdio.h>

#include <GalacticC.h>
#include <ini.h>

GReflectedClass* g_pINIFileClass;

extern "C" __declspec(dllexport) const char* MODULE_NAME = "ini";

extern "C" __declspec(dllexport) void* _cdecl RegisterModule(GReflectedNamespace* pNamespace, void* pReserved) {
	g_pINIFileClass = Galactic_ReflectedNamespace_NewClass(pNamespace, "INIFile");

	Galactic_ReflectedNamespace_RegisterFunction(pNamespace, "create", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		CSimpleIni *ini = new CSimpleIni();

		GRefCounted* pRef = Galactic_Referenceable_Create(g_pINIFileClass);
		Galactic_Referenceable_SetPrivate(pRef, (void*)ini);
		Galactic_NativeState_ReturnReferenceable(pState, pRef);
		Galactic_RefCounted_Release(pRef);

		return true;
	});

	Galactic_ReflectedNamespace_RegisterFunction(pNamespace, "delete", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_CheckReferenceable(pState, 0);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.delete: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.delete: missing pointer\n");
		}

		delete ini;

		ini = NULL;

		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "errno", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		int err = 0;

		if (!Galactic_NativeState_CheckInt32(pState, 0, err)) {
			return Galactic_NativeState_SetError(pState, "ini.errno: missing errno\n");
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

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "reset", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.reset: missing pointer\n");
		}

		const char *szDefaultValue = Galactic_NativeState_CheckString(pState, 2);
		if (!szDefaultValue) szDefaultValue = "";

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.reset: missing pointer\n");
		}

		ini->Reset();

		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});


	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "delete", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.delete: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.delete: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.delete: empty key name\n");
		}

		bool bRemoveEmpty = true;
		if (!Galactic_NativeState_CheckBoolean(pState, 2, bRemoveEmpty)) bRemoveEmpty = true;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.delete: missing pointer\n");
		}

		bool result = ini->Delete(szSection, szKey, bRemoveEmpty);
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "isEmpty", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.isEmpty: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.isEmpty: missing pointer\n");
		}

		bool result = ini->IsEmpty();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "isMultiKey", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.isMultiKey: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.isMultiKey: missing pointer\n");
		}

		bool result = ini->IsMultiKey();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "isMultiLine", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.isMultiLine: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.isMultiLine: missing pointer\n");
		}

		bool result = ini->IsMultiLine();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "isUnicode", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.isUnicode: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.isUnicode: missing pointer\n");
		}

		bool result = ini->IsUnicode();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "isUsingSpaces", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.isUsingSpaces: missing pointer\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.isUsingSpaces: missing pointer\n");
		}

		bool result = ini->UsingSpaces();
		if (result) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "setUnicode", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.setUnicode: missing pointer\n");
		}

		bool bUnicode = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 0, bUnicode)) {
			return Galactic_NativeState_SetError(pState, "ini.setUnicode: empty boolean value\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.setUnicode: missing pointer\n");
		}

		ini->SetUnicode(bUnicode);

		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "loadFile", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.loadFile: missing pointer\n");
		}

		const char *szFilename = Galactic_NativeState_CheckString(pState, 0);
		if (!szFilename) {
			return Galactic_NativeState_SetError(pState, "ini.loadFile: empty filename\n");
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		
		SI_Error result = ini->LoadFile(szFilename);
		Galactic_NativeState_ReturnInt32(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "saveFile", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.saveFile: missing pointer\n");
		}

		const char *szFilename = Galactic_NativeState_CheckString(pState, 0);
		if (!szFilename) {
			return Galactic_NativeState_SetError(pState, "ini.saveFile: empty filename\n");
		}

		bool bSign = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 1, bSign)) {
			bSign = false;
		}

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (ini->SaveFile(szFilename, true) < 0) {
			return Galactic_NativeState_SetError(pState, "ini.saveFile: error saving file %s\n", szFilename);
		}

		if (ini) delete ini;
		ini = NULL;

		Galactic_Referenceable_SetPrivate(pINIRef, nullptr);
		Galactic_NativeState_ReturnBoolean(pState, true);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "getBoolValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.getBoolValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.getBoolValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.getBoolValue: empty key name\n");
		}

		bool bDefaultValue = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 2, bDefaultValue)) bDefaultValue = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.getBoolValue: missing pointer\n");
		}
		
		bool result = ini->GetBoolValue(szSection, szKey, bDefaultValue);

		Galactic_NativeState_ReturnBoolean(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "getIntValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.getIntValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.getIntValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.getIntValue: empty key name\n");
		}

		int64_t nDefaultValue = 0;
		if (!Galactic_NativeState_CheckInt64(pState, 2, nDefaultValue)) nDefaultValue = 0;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.getIntValue: missing pointer\n");
		}

		int64_t result = ini->GetLongValue(szSection, szKey, nDefaultValue);

		Galactic_NativeState_ReturnInt64(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "getFloatValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.getFloatValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.getFloatValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.getFloatValue: empty key name\n");
		}

		double nDefaultValue = 0.0;
		if (!Galactic_NativeState_CheckDouble(pState, 2, nDefaultValue)) nDefaultValue = 0.0;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.getFloatValue: missing pointer\n");
		}

		double result = ini->GetDoubleValue(szSection, szKey, nDefaultValue);

		Galactic_NativeState_ReturnDouble(pState, result);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "getString", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.getString: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.getString: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.getString: empty key name\n");
		}

		const char *szDefaultValue = Galactic_NativeState_CheckString(pState, 2);
		if (!szDefaultValue) szDefaultValue = "";

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.getString: missing pointer\n");
		}

		const char *result = ini->GetValue(szSection, szKey, szDefaultValue);

		Galactic_NativeState_ReturnString(pState, result, strlen(result));

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "setBoolValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.setBoolValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.setBoolValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.setBoolValue: empty key name\n");
		}

		bool bValue = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 2, bValue)) {
			return Galactic_NativeState_SetError(pState, "ini.setBoolValue: empty boolean value\n");
		}

		char szComment[128];
		bool hasComment = false;
		if (Galactic_NativeState_CheckString(pState, 3)) {
			strcpy_s(szComment, ";");
			strcat(szComment, Galactic_NativeState_CheckString(pState, 3));

			hasComment = true;
		}

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.setBoolValue: missing pointer\n");
		}

		SI_Error result;
		if (hasComment) result = ini->SetBoolValue(szSection, szKey, bValue, szComment, bForceReplace);
		else result = ini->SetBoolValue(szSection, szKey, bValue, 0, bForceReplace);

		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "setIntValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.setIntValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.setIntValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.setIntValue: empty key name\n");
		}

		int64_t iValue = 0;
		if (!Galactic_NativeState_CheckInt64(pState, 2, iValue)) {
			return Galactic_NativeState_SetError(pState, "ini.setIntValue: empty integer value\n");
		}

		char szComment[128];
		bool hasComment = false;
		if (Galactic_NativeState_CheckString(pState, 3)) {
			strcpy_s(szComment, ";");
			strcat(szComment, Galactic_NativeState_CheckString(pState, 3));

			hasComment = true;
		}

		bool bUseHex = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bUseHex)) bUseHex = false;

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 5, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.setIntValue: missing pointer\n");
		}

		SI_Error result;
		if (hasComment) result = ini->SetLongValue(szSection, szKey, iValue, szComment, bUseHex, bForceReplace);
		else result = ini->SetLongValue(szSection, szKey, iValue, 0, bUseHex, bForceReplace);

		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "setFloatValue", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.setFloatValue: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.setFloatValue: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.setFloatValue: empty key name\n");
		}

		double nValue = 0.0;
		if (!Galactic_NativeState_CheckDouble(pState, 2, nValue)) {
			return Galactic_NativeState_SetError(pState, "ini.setFloatValue: empty float value\n");
		}

		char szComment[128];
		bool hasComment = false;
		if (Galactic_NativeState_CheckString(pState, 3)) {
			strcpy_s(szComment, ";");
			strcat(szComment, Galactic_NativeState_CheckString(pState, 3));

			hasComment = true;
		}

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.setFloatValue: missing pointer\n");
		}

		SI_Error result;
		if (hasComment) result = ini->SetDoubleValue(szSection, szKey, nValue, szComment, bForceReplace);
		else result = ini->SetDoubleValue(szSection, szKey, nValue, 0, bForceReplace);

		if (result == SI_OK) Galactic_NativeState_ReturnBoolean(pState, true);
		else Galactic_NativeState_ReturnBoolean(pState, false);

		return true;
	});

	Galactic_ReflectedClass_RegisterFunction(g_pINIFileClass, "setString", "", [](GNativeState* pState, int32_t argc, void* pUser) {
		GRefCounted *pINIRef = Galactic_NativeState_GetThis(pState, g_pINIFileClass);
		if (!pINIRef) {
			return Galactic_NativeState_SetError(pState, "ini.setString: missing pointer\n");
		}

		const char *szSection = Galactic_NativeState_CheckString(pState, 0);
		if (!szSection) {
			return Galactic_NativeState_SetError(pState, "ini.setString: empty sector name\n");
		}

		const char *szKey = Galactic_NativeState_CheckString(pState, 1);
		if (!szKey) {
			return Galactic_NativeState_SetError(pState, "ini.setString: empty key name\n");
		}

		const char *szValue = Galactic_NativeState_CheckString(pState, 2);
		if (!szValue) {
			return Galactic_NativeState_SetError(pState, "ini.setString: empty string value\n");
		}

		char szComment[128];
		bool hasComment = false;
		if (Galactic_NativeState_CheckString(pState, 3)) {
			strcpy_s(szComment, ";");
			strcat(szComment, Galactic_NativeState_CheckString(pState, 3));

			hasComment = true;
		}

		bool bForceReplace = false;
		if (!Galactic_NativeState_CheckBoolean(pState, 4, bForceReplace)) bForceReplace = false;

		CSimpleIni *ini = (CSimpleIni*)Galactic_Referenceable_GetPrivate(pINIRef);
		if (!ini) {
			return Galactic_NativeState_SetError(pState, "ini.setString: missing pointer\n");
		}

		SI_Error result;
		if (hasComment) result = ini->SetValue(szSection, szKey, szValue, szComment, bForceReplace);
		else result = ini->SetValue(szSection, szKey, szValue, 0, bForceReplace);

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