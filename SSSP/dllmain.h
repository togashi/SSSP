// dllmain.h : モジュール クラスの宣言

class __declspec(uuid("9F17AF32-0347-4DC0-8492-E3484F3123CF")) CSSSPModule : public CAtlDllModuleT< CSSSPModule >
{
public :
	DECLARE_LIBID(__uuidof(CSSSPModule))
	//DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SSSP, "{9BFD05BC-A021-402B-B4D5-DE5BE39CFB0A}")
};

extern class CSSSPModule _AtlModule;
