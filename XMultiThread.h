#pragma once

#include <process.h>

class CXMultiThread
{

public:

	CXMultiThread(void);

	~CXMultiThread(void);

	bool Init(LPTHREAD_START_ROUTINE MainFun,LPVOID pPara,bool bRunWhenCreate);

	void Resume();

	void Suspend();

	void Terminate();

public:

	DWORD mdwThredId; 

	GHandle mGHandle;
};

