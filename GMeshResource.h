#pragma once
#include "GGraph.h"
class GMeshResource:public CXDynaArray<GGraph*>
{
public:
	GMeshResource(void);
	~GMeshResource(void);

	bool loadFromFile(const char* fileName);
	inline DWORD getSetCount() const;
};

inline DWORD GMeshResource::getSetCount() const
{
	return this->size();
}

