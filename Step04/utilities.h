#pragma once
#include "StdAfx.h"

inline Acad::ErrorStatus
getNamedObjectsDictionary(AcDbDictionary*& pNODictionary);

inline Acad::ErrorStatus
addDictionary(AcDbDictionary*& pDictionary, AcDbObjectId& id, const ACHAR* name);

inline Acad::ErrorStatus
hasDictionary(AcDbDictionary*& pDictionary, AcDbObjectId& id, const ACHAR* name);

class Dictionary {
private:
	AcDbDictionary* pDictionary{ nullptr };
	AcDbObjectId objId{};
	const ACHAR* dictionaryName{ L"EMPLOYEE_DICTIONARY" };

public:
	inline bool hasDictionary() const;
	inline void initDictionary();
	inline Acad::ErrorStatus
		hasEntry(const ACHAR* entryName, AcDbObject*& entryObj) const;
	inline Acad::ErrorStatus addEntry(const ACHAR* entryName);
	inline Acad::ErrorStatus removeEntry(const ACHAR* entryName);
};




