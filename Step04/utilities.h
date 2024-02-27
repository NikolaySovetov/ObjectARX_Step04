#pragma once
#include "StdAfx.h"

Acad::ErrorStatus
getNamedObjectsDictionary
(AcDbDictionary*& pNODictionary, AcDb::OpenMode mode = AcDb::kForRead);

Acad::ErrorStatus
addDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary);

Acad::ErrorStatus
hasDictionary(const ACHAR* dictName);


Acad::ErrorStatus
getDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary,
	AcDb::OpenMode mode = AcDb::kForRead);

Acad::ErrorStatus
addEntry(const ACHAR* dictName, const ACHAR* entry);

Acad::ErrorStatus
removeEntry(const ACHAR* dictName, const ACHAR* entry);



class Dictionary {
private:
	//AcDbDictionary* pNODictionary{ nullptr };
	AcDbDictionary* pDictionary{ nullptr };
	AcDbObjectId objId{};
	const ACHAR* dictionaryName{ L"EMPLOYEE_DICTIONARY" };

public:
	bool hasDictionary() const;
	void initDictionary();
	Acad::ErrorStatus addEntry(const ACHAR* entryName);
	Acad::ErrorStatus removeEntry(const ACHAR* entryName);
};





