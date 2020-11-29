#include "pch.h"

#include <SimpleIni.h>

#include <stdio.h>

MODULE_MAIN("ini");

SDK::Class g_INIFileClass;

class CIni
{
public:
	CIni(CSimpleIni* pINI);
	~CIni();

	CSimpleIni* m_pINI;

	void Close();
};

CIni::CIni(CSimpleIni* pDB) :
	m_pINI(pDB)
{
}

CIni::~CIni()
{
	Close();
}

void CIni::Close()
{
	if (m_pINI != nullptr)
	{
		delete m_pINI;

		m_pINI = nullptr;
	}
}

void ModuleRegister()
{
	g_INIFileClass = SDK::Class("ini");

	SDK::RegisterFunction("new", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		CSimpleIni *ini = new CSimpleIni();

		SDK::ClassValue<CIni, g_INIFileClass> Value(new CIni(ini));
		State.Return(Value);

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("die", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pThis->Close();

		return true;

		SDK_ENDTRY;
	});

	SDK::RegisterFunction("errno", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		int err = 0;
		State.CheckNumber(0, err);

		switch (err) {
		case SI_OK:
			State.Return(SDK::StringValue("ok"));
			break;
		case SI_UPDATED:
			State.Return(SDK::StringValue("updated"));
			break;
		case SI_INSERTED:
			State.Return(SDK::StringValue("inserted"));
			break;
		case SI_FAIL:
			State.Return(SDK::StringValue("fail"));
			break;
		case SI_NOMEM:
			State.Return(SDK::StringValue("nomem"));
			break;
		case SI_FILE:
			//todo: probably extend this?
			State.Return(SDK::StringValue("file"));
			break;
		}

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("reset", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szDefaultValue = State.CheckString(2);
		if (!szDefaultValue) szDefaultValue = "";

		pThis->m_pINI->Reset();

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("delete", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.delete: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.delete: empty key name\n");
		}

		bool bRemoveEmpty = true;
		State.CheckBoolean(2, bRemoveEmpty);

		if (pThis->m_pINI->Delete(szSection, szKey, bRemoveEmpty)) return true;
		
		return false;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("isEmpty", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pState->ReturnBoolean(pThis->m_pINI->IsEmpty());

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("isMultiKey", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pState->ReturnBoolean(pThis->m_pINI->IsMultiKey());

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("isMultiLine", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pState->ReturnBoolean(pThis->m_pINI->IsMultiLine());

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("isUnicode", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pState->ReturnBoolean(pThis->m_pINI->IsUnicode());

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("isUnicode", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pState->ReturnBoolean(pThis->m_pINI->IsUnicode());

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("isUsingSpaces", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		pState->ReturnBoolean(pThis->m_pINI->UsingSpaces());

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("setUnicode", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		bool bUnicode = false;
		State.CheckBoolean(0, bUnicode);

		pThis->m_pINI->SetUnicode(bUnicode);

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("loadFile", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char* szFilename = State.CheckString(0);
		if (!szFilename) {
			return pState->SetError("ini.saveFile: empty filename\n");
		}

		SI_Error result = pThis->m_pINI->LoadFile(szFilename);
		SDK::NumberValue Value(result);
		State.Return(Value);

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("saveFile", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szFilename = State.CheckString(0);
		if (!szFilename) {
			return pState->SetError("ini.saveFile: empty filename\n");
		}

		if (pThis->m_pINI->SaveFile(szFilename, false) < 0) {
			return pState->SetError("ini.saveFile: error saving file %s\n", szFilename);
		}

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("getBoolValue", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.getBoolValue: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.getBoolValue: empty key name\n");
		}

		bool bDefaultValue = false;
		State.CheckBoolean(2, bDefaultValue);

		pState->ReturnBoolean(pThis->m_pINI->GetBoolValue(szSection, szKey, bDefaultValue));

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("getIntValue", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.getIntValue: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.getIntValue: empty key name\n");
		}

		int64_t nDefaultValue = 0;
		State.CheckNumber(2, nDefaultValue);

		long result = pThis->m_pINI->GetLongValue(szSection, szKey, (long)nDefaultValue);

		SDK::NumberValue Value(result);
		State.Return(Value);

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("getFloatValue", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.getFloatValue: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.getFloatValue: empty key name\n");
		}

		double nDefaultValue = 0;
		State.CheckNumber(2, nDefaultValue);

		double result = pThis->m_pINI->GetDoubleValue(szSection, szKey, nDefaultValue);

		SDK::NumberValue Value(result);
		State.Return(Value);

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("getString", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char* szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.getString: empty sector name\n");
		}

		const char* szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.getString: empty key name\n");
		}

		const char* szDefaultValue = State.CheckString(2);
		if (!szDefaultValue) szDefaultValue = "";

		const char* result = pThis->m_pINI->GetValue(szSection, szKey, szDefaultValue);

		State.Return(SDK::StringValue(result));

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("setBoolValue", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.setBoolValue: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.setBoolValue: empty key name\n");
		}

		bool bValue = false;
		State.CheckBoolean(2, bValue);

		SI_Error result = pThis->m_pINI->SetBoolValue(szSection, szKey, bValue, 0, false);
		State.Return(SDK::NumberValue(result));

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("setIntValue", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.setIntValue: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.setIntValue: empty key name\n");
		}

		int64_t nValue = 0;
		State.CheckNumber(2, nValue);

		bool bUseHex = false;
		State.CheckBoolean(3, bUseHex);

		SI_Error result = pThis->m_pINI->SetLongValue(szSection, szKey, (long)nValue, 0, bUseHex, false);
		State.Return(SDK::NumberValue(result));

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("setFloatValue", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.setFloatValue: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.setFloatValue: empty key name\n");
		}

		double nValue = 0.0;
		State.CheckNumber(2, nValue);

		SI_Error result = pThis->m_pINI->SetDoubleValue(szSection, szKey, nValue, 0, false);
		State.Return(SDK::NumberValue(result));

		return true;

		SDK_ENDTRY;
	});

	g_INIFileClass.RegisterFunction("setString", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CIni, g_INIFileClass>();

		const char *szSection = State.CheckString(0);
		if (!szSection) {
			return pState->SetError("ini.setString: empty sector name\n");
		}

		const char *szKey = State.CheckString(1);
		if (!szKey) {
			return pState->SetError("ini.setString: empty key name\n");
		}

		const char *szValue = State.CheckString(2);
		if (!szValue) {
			return pState->SetError("ini.setString: empty string value\n");
		}

		SI_Error result = pThis->m_pINI->SetValue(szSection, szKey, szValue, 0, false);
		State.Return(SDK::NumberValue(result));

		return true;

		SDK_ENDTRY;
	});

	printf("[INI] Module loaded \n");
	printf("[INI] Build date: %s \n", __DATE__);
}

void ModuleUnregister()
{

}
