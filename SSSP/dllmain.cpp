// dllmain.cpp : DllMain の実装

#include "stdafx.h"
#include "resource.h"
//#include "SSSP_i.h"
#include "dllmain.h"

CSSSPModule _AtlModule;

ULONG_PTR g_gdipctx = 0;

// DLL エントリ ポイント
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		::OleInitialize(NULL);
	} else if (dwReason == DLL_PROCESS_DETACH) {
		::OleUninitialize();
	}
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
