#include "StdAfx.h"
#include "utilities.h"

inline Acad::ErrorStatus
getNamedObjectsDictionary(AcDbDictionary*& pNODictionary) {

	Acad::ErrorStatus errStat;

	errStat = acdbHostApplicationServices()->
		workingDatabase()->getNamedObjectsDictionary(pNODictionary, AcDb::kForRead);

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't get Named Objects Dictionary");
		return errStat;
	}

	return errStat;
}

inline Acad::ErrorStatus
addDictionary(AcDbDictionary*& pDictionary, AcDbObjectId& id, const ACHAR* name) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pNODictionary;

	errStat = getNamedObjectsDictionary(pNODictionary);
	if (errStat != Acad::eOk)
		return errStat;

	pDictionary = new AcDbDictionary;

	if ((errStat = pNODictionary->upgradeOpen()) != Acad::eOk) {
		acutPrintf(L"\nError: Can't open to write the Named Objects Dictionary");
		delete pDictionary;
		pDictionary = nullptr;
		return errStat;
	}

	errStat = pNODictionary->setAt(name, pDictionary, id);
	pNODictionary->close();

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't add the Dictionary");
		delete pDictionary;
		pDictionary = nullptr;
		return errStat;
	}

	return errStat;
}

//-----------------------------------------------------------------
inline bool Dictionary::hasDictionary() const {

	return mPtrDictionary != nullptr;
}

inline void Dictionary::initDictionary() {

	if (addDictionary(mPtrDictionary, mId, mName) == Acad::eOk) {
		mPtrDictionary->close();
	}
}

inline Acad::ErrorStatus
Dictionary::hasEntry(const ACHAR* entryName, AcDbObject*& pEntryObj) const {

	Acad::ErrorStatus errStat;
	//AcDbObject* pEntry;

	// HACK: need I open Dictionary before getAt() ???
	errStat = mPtrDictionary->getAt(entryName, pEntryObj, AcDb::kForRead);
	pEntryObj->close();

	if (errStat == Acad::eInvalidKey) {
		acutPrintf(L"\nWarning: Key is invalid");
		return errStat;
	}
	else if (errStat == Acad::eOk) {
		acutPrintf(L"\nWarning: This entry is registered");
		return errStat;
	}

	return errStat;
}

inline Acad::ErrorStatus Dictionary::addEntry(const ACHAR* entryName) {

	AcDbObject* pEntryObj;
	Acad::ErrorStatus errStat;

	errStat = hasEntry(entryName, pEntryObj);
	if (errStat != Acad::eKeyNotFound) {
		return errStat;
	}

	//AcDbObject* pDictObj = reinterpret_cast<AcDbObject*>(mPtrDictionary);
	errStat = acdbOpenAcDbObject(reinterpret_cast<AcDbObject*&>(mPtrDictionary),
								 mId, AcDb::kForWrite);

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't open Dictionary");
		return errStat;
	}

	AcDbXrecord* newRecord = new AcDbXrecord;






}



