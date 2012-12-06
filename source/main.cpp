#include "stdafx.h"
#include <dbgeng.h>
#include <sstream>

#define EXT_MAJOR_VER 0
#define EXT_MINOR_VER 1

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport) HRESULT

DWORD error = ERROR_DLL_INIT_FAILED;

EXTERN_DLL_EXPORT DebugExtensionInitialize(PULONG Version, PULONG Flags)
{
	*Version = DEBUG_EXTENSION_VERSION(EXT_MAJOR_VER, EXT_MINOR_VER);
	*Flags = 0;

	if (SetProcessShutdownParameters(100, 0))
	{
		error = ERROR_SUCCESS;
	}
	else
	{
		error = GetLastError();
	}

	return S_OK;
}

EXTERN_DLL_EXPORT help(PDEBUG_CLIENT pDebugClient, PCSTR args)
{
	IDebugControl *pDebugControl;
	if (SUCCEEDED(pDebugClient->QueryInterface(__uuidof(IDebugControl), (void **)&pDebugControl)))
	{
		if (error == ERROR_SUCCESS)
		{
			pDebugControl->Output(DEBUG_OUTPUT_NORMAL, "Going to die last\n");
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "Error " << std::hex << std::showbase << error << "\n";
			pDebugControl->Output(DEBUG_OUTPUT_NORMAL, errorMessage.str().c_str());
		}

		pDebugControl->Release();
	}

	return S_OK;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	return TRUE;
}
