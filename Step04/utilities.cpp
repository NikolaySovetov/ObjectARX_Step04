#include "StdAfx.h"
#include "utilities.h"

Acad::ErrorStatus
getNamedObjectsDictionary(AcDbDictionary*& pNODictionary, AcDb::OpenMode mode) {

	Acad::ErrorStatus errStat;

	errStat = acdbHostApplicationServices()->
		workingDatabase()->getNamedObjectsDictionary(pNODictionary, mode);

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't get Named Objects Dictionary");
		return errStat;
	}

	return errStat;
}

Acad::ErrorStatus
hasDictionary(const ACHAR* dictName) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pNODictionary;

	errStat = getNamedObjectsDictionary(pNODictionary, AcDb::kForRead);
	if (errStat != Acad::eOk)
		return errStat;

	AcDbObjectId id;
	errStat = pNODictionary->getAt(dictName, id);
	pNODictionary->close();

	if (errStat == Acad::eOk) {
		acutPrintf(_T("\nWarning: \"%s\" registered."), dictName);
		return errStat;
	}
	else if (errStat == Acad::eInvalidKey) {
		acutPrintf(_T("\nWarning: \"%s\" invalid key."), dictName);
		return errStat;
	}

	return Acad::eKeyNotFound;
}

Acad::ErrorStatus
addDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pNODictionary;

	errStat = getNamedObjectsDictionary(pNODictionary, AcDb::kForWrite);
	if (errStat != Acad::eOk)
		return errStat;

	try {
		pDictionary = new AcDbDictionary;
	}
	catch (const std::exception& ex) {
		acutPrintf(_T("\nException: %s"), ex.what());
		pNODictionary->close();
		delete pDictionary;
		throw;
	}

	AcDbObjectId id;
	errStat = pNODictionary->setAt(dictName, pDictionary, id); //*******
	pNODictionary->close();

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't add the Dictionary");
		delete pDictionary;
		pDictionary = nullptr;
		return errStat;
	}

	return errStat;
}

Acad::ErrorStatus
getDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary, AcDb::OpenMode mode) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pNODictionary;

	errStat = getNamedObjectsDictionary(pNODictionary, AcDb::kForRead);
	if (errStat != Acad::eOk) {
		return errStat;
	}

	AcDbObjectId id;

	errStat = pNODictionary->getAt(dictName, pDictionary, mode);
	if (errStat != Acad::eOk) {
		acutPrintf(_T("\nError: Can't open \"%s\" dictionary"), dictName);
		return errStat;
	}

	return errStat;
}

Acad::ErrorStatus
addEntry(const ACHAR* dictName, const ACHAR* entry) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pDictionary;

	errStat = getDictionary(dictName, pDictionary, AcDb::kForWrite);
	if (errStat != Acad::eOk) {
		return errStat;
	}

	AcDbXrecord* pRecord;
	try {
		pRecord = new AcDbXrecord;
	}
	catch (const std::exception& ex) {
		acutPrintf(_T("\nException: %s"), ex.what());
		pDictionary->close();
		throw;
	}

	AcDbObjectId id;
	errStat = pDictionary->setAt(entry, pRecord, id);
	pDictionary->close();
	if (errStat != Acad::eOk) {
		acutPrintf(_T("\nError: Can't add \"%s\" to \"%s\"."), entry, dictName);
		delete pRecord;
		return errStat;
	}

	pRecord->close();

	return errStat;
}

Acad::ErrorStatus
removeEntry(const ACHAR* dictName, const ACHAR* entry) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pDictionary;

	errStat = getDictionary(dictName, pDictionary, AcDb::kForRead);
	if (errStat != Acad::eOk) {
		return errStat;
	}

	AcDbObject* pEntryObj;
	errStat = pDictionary->getAt(entry, pEntryObj, AcDb::kForWrite);
	pDictionary->close();

	if (errStat != Acad::eOk) {
		acutPrintf(_T("\nWarning: Can't find \"%s\" in the \"%s\"."), entry, dictName);
		return errStat;
	}

	pEntryObj->erase();
	pEntryObj->close();

	return errStat;
}






//-----------------------------------------------------------------
bool Dictionary::hasDictionary() const {

	return pDictionary != nullptr;
}

void Dictionary::initDictionary() {


}

Acad::ErrorStatus Dictionary::addEntry(const ACHAR* entryName) {

	Acad::ErrorStatus errStat;
	AcDbObject* pEntryObj;

	// Check if entry is registered
	errStat = pDictionary->getAt(entryName, pEntryObj, AcDb::kForRead);

	if (errStat == Acad::eInvalidKey) {
		acutPrintf(L"\nWarning: Key is invalid");
		return errStat;
	}
	else if (errStat == Acad::eOk) {
		acutPrintf(L"\nWarning: This entry is registered");
		pEntryObj->close();
		return errStat;
	}

	// If not registered then add new empty record
	errStat = acdbOpenAcDbObject((AcDbObject*&)pDictionary, objId, AcDb::kForWrite);

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't open Dictionary to add new record.");
		return errStat;
	}

	AcDbXrecord* pRecord = new AcDbXrecord;
	AcDbObjectId recordId;

	errStat = pDictionary->setAt(entryName, pRecord, recordId);
	pDictionary->close();

	if (errStat != Acad::eOk) {
		acutPrintf(L"\nError: Can't add new record to dictionary.");
		delete pRecord;
		return errStat;
	}

	pRecord->close();

	return errStat;
}

Acad::ErrorStatus Dictionary::removeEntry(const ACHAR* entryName) {

	Acad::ErrorStatus errStat;
	AcDbObject* pEntryObj;

	// Check if entry is registered
	errStat = pDictionary->getAt(entryName, pEntryObj, AcDb::kForWrite);

	if (errStat == Acad::eInvalidKey) {
		acutPrintf(L"\nWarning: Can't remove the entry. Key is invalid.");
		return errStat;
	}
	else if (errStat == Acad::eKeyNotFound) {
		acutPrintf(L"\nWarning: Can't remove the entry. Key not found.");
		return errStat;
	}

	pEntryObj->erase();
	pEntryObj->close();

	return errStat;
}


