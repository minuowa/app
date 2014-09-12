#pragma once


class GFrameWork:public CXCallBack
{
public:
    GFrameWork(void);
    virtual ~GFrameWork(void);
public:
	void active(bool active);
	bool isActive() const;
    virtual bool initBase(HWND mainWnd);//初始化游戏  
	HWND getMainWin() const { return mMainWin; }
	void resize(int w,int h);
public:
    HWND mMainWin;//主窗口句柄
	HINSTANCE mInst;//Game进程 
	bool mIsActive;
};

//窗口函数声明
 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);