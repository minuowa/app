#ifndef GDevice_D3D_h__
#define GDevice_D3D_h__

#include "GDevice.h"
//-------------------------------------------------------------------------
/**
	@brief
	@author nnboy,17/9/2014  22:49
*/
class GDevice_D3D: public GDevice
{
public:
    GDevice_D3D ( void );
    ~GDevice_D3D ( void );

private:

    IDirect3DDevice9 *mD9Device;
    bool mIsLosing;
public:

    HWND mhWnd;

    int mWidth;	    //渲染区域宽度
    int mHeight;	    //渲染区域高度

    D3DLIGHT9 mLight[LIGHT_NUM];

    D3DCAPS9 mCaps;

    IDirect3D9 *mD3D9;
public:
    int GetScreenWidth() const;
    int GetScreenHeight() const;

    bool Init ( HWND hWnd ); //初始化3D设备

    virtual bool beginRender() override;
    virtual void endRender() override;

    void SetLight();	    //设置初始灯光

    void OpenLight ( int nIndex, bool bOpen, bool bUseMatrialColor = false ); //开启或者关闭指定的光源

    void OpenlightEx ( int nIndex, bool bOpen, float fDiffuseIntensity, bool bUseMatrialColor = false );

    void OpenAllLight ( bool bOpen, bool bUseMatrialColor = false ); //开启所有光源

    void OpenAllLightEx ( bool bOpen, float fDiffuseIntensity, float fAmbientIntensity, float fSpecularIntensity, bool bUseMatrialColor = false ); //设置指定参数光源，并开启

    void OpenAlphaBlend ( bool bOpen );	//开启Alpha混合

    void ResetRenderState();

    void BeginMirrorRender();

    void OpenUIAlpha ( bool bTransparent );

    bool VertexShaderSurport() const;
    virtual void setMaterial ( D3DMATERIAL9* mat ) override;

    inline IDirect3DDevice9 *getDVC() const
    {
        return mD9Device;
    }
    inline int TestDevice();
    void OnDeviceLost();
    void OnResize ( int w, int h );
protected:
    bool ResetDevice ( int w, int h );

    virtual GDeviceType getType() override;

};
typedef CXSingleton<GDevice_D3D> GSingletonD9Device;
inline int GDevice_D3D::TestDevice()
{
    return mD9Device->TestCooperativeLevel();
}
#endif // GDevice_D3D_h__