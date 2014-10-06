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

    int mWidth;	    //��Ⱦ������
    int mHeight;	    //��Ⱦ����߶�

    D3DLIGHT9 mLight[LIGHT_NUM];

    D3DCAPS9 mCaps;

    IDirect3D9 *mD3D9;
public:
    int GetScreenWidth() const;
    int GetScreenHeight() const;

    bool Init ( HWND hWnd ); //��ʼ��3D�豸

    virtual bool beginRender() override;
    virtual void endRender() override;

    void SetLight();	    //���ó�ʼ�ƹ�

    void OpenLight ( int nIndex, bool bOpen, bool bUseMatrialColor = false ); //�������߹ر�ָ���Ĺ�Դ

    void OpenlightEx ( int nIndex, bool bOpen, float fDiffuseIntensity, bool bUseMatrialColor = false );

    void OpenAllLight ( bool bOpen, bool bUseMatrialColor = false ); //�������й�Դ

    void OpenAllLightEx ( bool bOpen, float fDiffuseIntensity, float fAmbientIntensity, float fSpecularIntensity, bool bUseMatrialColor = false ); //����ָ��������Դ��������

    void OpenAlphaBlend ( bool bOpen );	//����Alpha���

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