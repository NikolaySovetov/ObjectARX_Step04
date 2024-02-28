//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep04App : public AcRxArxApp {

public:
	CStep04App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void Step04_addDictionary() {

		const TCHAR* dictName{ L"EMPLOYEE_DICTIONARY" };
		AcDbDictionary* pDictionary;

		try {
			if (addDictionary(dictName, pDictionary) == Acad::eOk) {
				acutPrintf(_T("\n\"%s\" dictionary was added."), dictName);
			}
		}
		catch (const std::exception&) {
			acutPrintf(_T("\nError: Can't add \"%s\" dictionary "), dictName);
		}
	}

	static void Step04_addentry() {

		const TCHAR* dictName{ L"EMPLOYEE_DICTIONARY" };
		TCHAR customAnswer[128];

		int error = acedGetString(0, _T("\nEnter employee name: "), customAnswer);
		if (error != RTNORM) {
			acutPrintf(_T("\n... some error occur: %d "), error);
			return;
		}

		try {
			if (addEntry(dictName, customAnswer) == Acad::eOk) {
				acutPrintf(_T("\n\"%s\" was added to \"%s\"."), customAnswer, dictName);
			}
		}
		catch (const std::exception&) {
		}
	}

	static void Step04_listentries() {

		const TCHAR* dictName{ L"EMPLOYEE_DICTIONARY" };

		listentries(dictName);
	}

	static void Step04_removeentry() {

		const TCHAR* dictName{ L"EMPLOYEE_DICTIONARY" };
		TCHAR customAnswer[128];

		int error = acedGetString(0, _T("\nEnter employee name: "), customAnswer);
		if (error != RTNORM) {
			acutPrintf(_T("\n... has occur some error: %d "), error);
			return;
		}

		if (removeEntry(dictName, customAnswer) == Acad::eOk) {
			acutPrintf(_T("\n\"%s\" removed form the \"%s\" "), customAnswer, dictName);
		}

	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep04App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, Step04, _addentry, addentry, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, Step04, _addDictionary, addDictionary, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, Step04, _listentries, listentries, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, Step04, _removeentry, removeentry, ACRX_CMD_TRANSPARENT, NULL)

