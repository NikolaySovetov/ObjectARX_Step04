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
addDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pNODictionary;

	errStat = getNamedObjectsDictionary(pNODictionary, AcDb::kForWrite);
	if (errStat != Acad::eOk)
		return errStat;

	if (pNODictionary->has(dictName)) {
		pNODictionary->close();
		acutPrintf(_T("\n\"%s\" registered."), dictName);
		return Acad::eOk;
	}

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
	pDictionary->close();

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
	pNODictionary->close();

	if (errStat != Acad::eOk) {
		acutPrintf(_T("\nError: Can't open \"%s\" dictionary"), dictName);
		return errStat;
	}

	return errStat;
}

Acad::ErrorStatus
addEntry(const ACHAR* dictName, const ACHAR* entryName) {

	Acad::ErrorStatus errStat;
	
	AcDbDictionary* pDictionary;
	errStat = getDictionary(dictName, pDictionary, AcDb::kForWrite);
	if (errStat != Acad::eOk) {
		return errStat;
	}

	if (pDictionary->has(entryName)) {
		acutPrintf(_T("\nWarning: \"%s\" registered in \"%s\"."), entryName, dictName);
		return Acad::eOk;
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
	errStat = pDictionary->setAt(entryName, pRecord, id);
	pDictionary->close();
	if (errStat != Acad::eOk) {
		acutPrintf(_T("\nError: Can't add \"%s\" to \"%s\"."), entryName, dictName);
		delete pRecord;
		return errStat;
	}

	pRecord->close();

	return errStat;
}

Acad::ErrorStatus
removeEntry(const ACHAR* dictName, const ACHAR* entryName) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pDictionary;

	errStat = getDictionary(dictName, pDictionary, AcDb::kForRead);
	if (errStat != Acad::eOk) {
		return errStat;
	}

	AcDbObject* pEntryObj;
	errStat = pDictionary->getAt(entryName, pEntryObj, AcDb::kForWrite);
	pDictionary->close();

	if (errStat != Acad::eOk) {
		acutPrintf(_T("\nWarning: Can't find \"%s\" in the \"%s\"."), entryName, dictName);
		return errStat;
	}

	pEntryObj->erase();
	pEntryObj->close();

	return errStat;
}

Acad::ErrorStatus
listentries(const ACHAR* dictName) {

	Acad::ErrorStatus errStat;
	AcDbDictionary* pDictionary;

	errStat = getDictionary(dictName, pDictionary, AcDb::kForRead);
	if (errStat != Acad::eOk) {
		return errStat;
	}

	AcDbDictionaryIterator* pDIterator = pDictionary->newIterator();

	int num{ 1 };

	for (; !pDIterator->done(); pDIterator->next()) {
		acutPrintf(_T("\n%d. \"%s\"."), num, pDIterator->name());
		++num;
	}
	delete pDIterator;

	pDictionary->close();

	return Acad::eOk;
}


