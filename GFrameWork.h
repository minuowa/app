#pragma once


class GFrameWork
{
public:
    GFrameWork(void);
    virtual ~GFrameWork(void);
public:
	void Active(bool active);
	bool IsActive() const;
    virtual bool InitBase(HWND mainWnd);//初始化游戏  
	HWND GetMainWin() const { return mMainWin; }
public:
    HWND mMainWin;//主窗口句柄
	HINSTANCE mInst;//Game进程 
	bool mIsActive;
};

//窗口函数声明
 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);