#pragma once
#include "StdAfx.h"

Acad::ErrorStatus
getNamedObjectsDictionary
(AcDbDictionary*& pNODictionary, AcDb::OpenMode mode = AcDb::kForRead);

Acad::ErrorStatus
addDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary);

Acad::ErrorStatus
hasDictionary(const ACHAR* dictName, bool& hasObjectFlag);

Acad::ErrorStatus
getDictionary(const ACHAR* dictName, AcDbDictionary*& pDictionary,
	AcDb::OpenMode mode = AcDb::kForRead);

Acad::ErrorStatus
hasEntry
(const ACHAR* dictName, const ACHAR* entryName, bool& hasObjectFlag);

Acad::ErrorStatus
addEntry(const ACHAR* dictName, const ACHAR* entryName);

Acad::ErrorStatus
removeEntry(const ACHAR* dictName, const ACHAR* entryName);

Acad::ErrorStatus
listentries(const ACHAR* dictName);



