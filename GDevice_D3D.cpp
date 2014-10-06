#include "GGameDemoHeader.h"
#include "GDevice_D3D.h"
#include <assert.h>

GDevice_D3D::GDevice_D3D(void)
{
	mD9Device=NULL;
	mD3D9=0;
	mIsLosing=false;
}

GDevice_D3D::~GDevice_D3D(void)
{

}

bool GDevice_D3D::Init( HWND hWnd )
{
	mhWnd=hWnd;
	//��̨�������Ϳͻ�����Сһ�£�����ͼ��ʧ��
	RECT rcClient;
	GetClientRect(hWnd,&rcClient);
	mHeight=rcClient.bottom-rcClient.top;
	mWidth=rcClient.right-rcClient.left;

	return ResetDevice(mWidth,mHeight);
}

bool GDevice_D3D::beginRender()
{
	if (mD9Device==NULL ||mIsLosing)
		return false;

	
	//��һ��ָ������ɫ����ӿڣ������ӿ��е�һ���������������Ȼ����ģ�建��

	//RECT rc={300,300,500,330};
	//D3DRECT d3dRc[4];
	//d3dRc[0].x1=0;
	//d3dRc[0].y1=0;
	//d3dRc[0].x2=300;
	//d3dRc[0].y2=700;

	//d3dRc[1].x1=500;
	//d3dRc[1].y1=0;
	//d3dRc[1].x2=1008;
	//d3dRc[1].y2=700;

	//d3dRc[2].x1=300;
	//d3dRc[2].y1=0;
	//d3dRc[2].x2=500;
	//d3dRc[2].y2=330;

	//d3dRc[3].x1=300;
	//d3dRc[3].y1=330;
	//d3dRc[3].x2=500;
	//d3dRc[3].y2=700;

	if(FAILED(mD9Device->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0x00000000,1.0f,0)))
	{
		assert(0 && "Clear device failed!");
		return false;
	}

	//D9DEVICE->Clear(2,&d3dRc[0],D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0x99999999,1.0f,0);

	if(FAILED(mD9Device->BeginScene()))
	{
		return false;
	}

	return true;
}

void GDevice_D3D::endRender()
{
	if (mD9Device!=NULL)
	{
		mD9Device->EndScene();
	}

	if(D3DERR_DEVICELOST==mD9Device->Present(0,0,mhWnd,0))
	{
		CXASSERT(0);
	}
}


void GDevice_D3D::SetLight()
{
	ZeroMemory(mLight,sizeof(mLight));
	for (int i=0;i<LIGHT_NUM;i++)
	{
		mLight[i].Type=D3DLIGHT_DIRECTIONAL;

		mLight[i].Diffuse.a=DIFFUSE_INTENSITY;
		mLight[i].Diffuse.r=DIFFUSE_INTENSITY;
		mLight[i].Diffuse.g=DIFFUSE_INTENSITY;
		mLight[i].Diffuse.b=DIFFUSE_INTENSITY;

		mLight[i].Ambient.a=AMBIENT_INTENSIYT;
		mLight[i].Ambient.r=AMBIENT_INTENSIYT;
		mLight[i].Ambient.g=AMBIENT_INTENSIYT;
		mLight[i].Ambient.b=AMBIENT_INTENSIYT;

		mLight[i].Specular.a=SPECULAR_INTENSITY;
		mLight[i].Specular.r=SPECULAR_INTENSITY;
		mLight[i].Specular.g=SPECULAR_INTENSITY;
		mLight[i].Specular.b=SPECULAR_INTENSITY;
	}

		mLight[0].Direction=Vector(1.0f,-0.5f,0.0f);

	//mLight[0].Direction=Vector(-3.0f,-2.0f,-1.732f);	
	//mLight[1].Direction=Vector(3.0f,-2.0f,-1.732f);	
	//mLight[2].Direction=Vector(0.0f,-2.0f,3.464f);	
	//mLight[3].Direction=Vector(0.0f,4.0f,0.0f);
	//mLight[3].Direction=Vector(0.0f,-4.0f,0.0f);

	if (mD9Device!=NULL)
	{
		for (int i=0;i<LIGHT_NUM;i++)
		{
			mD9Device->SetLight(i,&mLight[i]);
		}
	}

}



void GDevice_D3D::OpenLight( int nIndex,bool bOpen,bool bUseMatrialColor/*=false*/ )
{
	mD9Device->SetRenderState(D3DRS_LIGHTING,bOpen);
	mD9Device->LightEnable(nIndex,bOpen);
	mD9Device->SetRenderState(D3DRS_AMBIENT,0xff4a4a4a);
	mD9Device->SetRenderState(D3DRS_SPECULARENABLE,bOpen);

	//����������ɫΪ��һ��������������ɫΪ�ڶ�����
	mD9Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	mD9Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

	if (bUseMatrialColor)
	{
		mD9Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTSS_COLORARG2);
	}
	else
	{
		if (bOpen)
		{
			mD9Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);//���˹��գ�ʹ�����߻����ɫ
		}
		else
		{
			mD9Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTSS_COLORARG1);//û�����գ�ֻʹ��������ɫ
		}
	}
}



void GDevice_D3D::OpenAllLight( bool bOpen,bool bUseMatrialColor/*=false*/ )
{
	if (bOpen)
	{
		for (int i=0;i<LIGHT_NUM;i++)
		{
			OpenLight(i,bOpen,bUseMatrialColor);
		}
	}
	else
	{
		mD9Device->SetRenderState(D3DRS_LIGHTING,false);
	}
}



void GDevice_D3D::OpenAllLightEx( bool bOpen,float fDiffuseIntensity,float fAmbientIntensity, float fSpecularIntensity,bool bUseMatrialColor/*=false*/ )
{
	mD9Device->SetRenderState(D3DRS_LIGHTING,bOpen);

	if(bOpen)
	{

		for (int i=0;i<LIGHT_NUM;i++)
		{
			if (fDiffuseIntensity!=ZEROFLOAT)
			{
				mLight[i].Diffuse.r=fDiffuseIntensity;
				mLight[i].Diffuse.g=fDiffuseIntensity;
				mLight[i].Diffuse.b=fDiffuseIntensity;

				mD9Device->SetLight(i,&mLight[i]);
				OpenLight(i,bOpen,bUseMatrialColor);
			}

		}

	}
}

void GDevice_D3D::OpenAlphaBlend( bool bOpen )
{
	mD9Device->SetRenderState(D3DRS_ALPHABLENDENABLE,bOpen);

	if (bOpen)
	{
		mD9Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		mD9Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

		mD9Device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
		mD9Device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
		mD9Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);

		////��ɫ��ϣ���͸��
		//D9DEVICE->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
		//D9DEVICE->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
	}
}


void GDevice_D3D::ResetRenderState()
{
	//�رձ����ѡ,ʹ���涼�ܿ���,Ĭ��˳ʱ�뷽��ķ���Ϊ������ʹ�������жϹ���
	mD9Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	//������Ȼ���
	mD9Device->SetRenderState(D3DRS_ZENABLE,true);
	mD9Device->SetRenderState(D3DRS_ZWRITEENABLE,true);

	//������ز�����ȫ�������
	mD9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,true);

	//���ø��޵���ɫģʽ
	mD9Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);


	//��С���Ŵ����ģʽ�²�������������˲���
	mD9Device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	mD9Device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	//D9DEVICE->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	//���ģʽ
	//D9DEVICE->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);//�����
	mD9Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);//��������
	//D9DEVICE->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);//�߿����

	//�ر�Alpha���
	mD9Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

	//�رչ���
	mD9Device->SetRenderState(D3DRS_LIGHTING,false);

	//�رջ�����
	mD9Device->SetRenderState(D3DRS_AMBIENT,0x4a4a4a4a);

	//�رվ��淴��
	mD9Device->SetRenderState(D3DRS_SPECULARENABLE,false);

	//������������Ѱַ��ʽΪѭ��Ѱַ
	mD9Device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	mD9Device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	//D9DEVICE->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);//������������任
	//D9DEVICE->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);//ʹ�ö�������������

	D3DMATERIAL9 matr;
	ZeroMemory(&matr,sizeof(D3DMATERIAL9));
	mD9Device->SetMaterial(&matr);
	//�ر���������任
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	mD9Device->SetTransform(D3DTS_TEXTURE0,&mat);

	//�ö�����ɫ��Ϊ����
	//D9DEVICE->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_COLOR1);
}


void GDevice_D3D::OpenlightEx( int nIndex,bool bOpen,float fDiffuseIntensity,bool bUseMatrialColor/*=false*/ )
{
	if (fDiffuseIntensity!=ZEROFLOAT)
	{
		mLight[nIndex].Diffuse.r=fDiffuseIntensity;
		mLight[nIndex].Diffuse.g=fDiffuseIntensity;
		mLight[nIndex].Diffuse.b=fDiffuseIntensity;
	}

	mD9Device->SetLight(nIndex,&mLight[nIndex]);

	OpenLight(nIndex,bOpen,bUseMatrialColor);
}

void GDevice_D3D::BeginMirrorRender()
{
	mD9Device->Clear(0,0,D3DCLEAR_STENCIL|D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0x000000000,1.0f,0);

	mD9Device->SetRenderState(D3DRS_STENCILENABLE,true);

	mD9Device->SetRenderState(D3DRS_STENCILREF,0x1);

	mD9Device->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);

}

void GDevice_D3D::OpenUIAlpha( bool bTransparent )
{	
	//����Alpha���
		
	mD9Device->SetRenderState( D3DRS_ALPHABLENDENABLE, bTransparent );

	if (bTransparent)
	{

		mD9Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
		mD9Device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);


		//����Alpha��Ϸ����ͻ��ϵ��

		mD9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD );  //Ĭ������
		mD9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		mD9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

bool GDevice_D3D::VertexShaderSurport() const
{
	return mCaps.VertexShaderVersion >= D3DVS_VERSION(2,0);
}

int GDevice_D3D::GetScreenWidth() const
{
	return mWidth;
}

int GDevice_D3D::GetScreenHeight() const
{
	return mHeight;
}

void GDevice_D3D::OnDeviceLost()
{
	mOnLostDevice.trigger();
}

void GDevice_D3D::OnResize( int w,int h )
{
	if (w && h)
	{
		mOnLostDevice.trigger();
		ResetDevice(w,h);
		mOnResetDevice.trigger();
	}
}

bool GDevice_D3D::ResetDevice( int w,int h )
{
	//������
	HRESULT hr=0;

	//Ĭ���Կ�����ʾģʽ
	D3DDISPLAYMODE d9DisplayMode;

	//�豸���ܲ���

	//��Ⱦ����
	RECT rcClient;

	//���ز���֧�ֶ�
	DWORD dwMultiSampleQulity=0;

	//���㴦��ʽ
	DWORD dwBehavior;

	if (!mD3D9)
	{
		mD3D9=Direct3DCreate9(D3D_SDK_VERSION);

		CXASSERT_RETURN_FALSE(mD3D9);
	}

	//����豸��ʾģʽ
	mD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d9DisplayMode);

	//D3DVS_VERSION()
	//����豸���ܣ��Ƿ�֧��Ӳ�����㴦��͹���
	//����豸֧�ֶ���ת���͹��գ���Ӳ���������������ģ��
	mD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&mCaps);

	if(mCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwBehavior=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehavior=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//�����ز���֧�ֶ�
	mD3D9->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d9DisplayMode.Format,
		TRUE,D3DMULTISAMPLE_8_SAMPLES,&dwMultiSampleQulity
		);
	mWidth=w;
	mHeight=h;
	D3DPRESENT_PARAMETERS d3dp;
	ZeroMemory(&d3dp,sizeof(D3DPRESENT_PARAMETERS));

	d3dp.AutoDepthStencilFormat=D3DFMT_D24S8;	//��Ȼ���24λ��ģ�建��8λ
	d3dp.BackBufferCount=1;						//��̨������һ��
	d3dp.BackBufferFormat=d9DisplayMode.Format;	//��̨���������ظ�ʽ��Ӧ�ú�ǰ̨��ͬ��
	d3dp.BackBufferHeight=mHeight;				//��̨�������߶�
	d3dp.BackBufferWidth=mWidth;				//��̨���������
	d3dp.EnableAutoDepthStencil=TRUE;			//����ģ�建�����Ȼ���
	d3dp.Flags=0;								//Ĭ��Ϊ0
	d3dp.FullScreen_RefreshRateInHz=0;			//������ʾʱ��������Ϊ0
	d3dp.hDeviceWindow=mhWnd;
	d3dp.MultiSampleQuality=dwMultiSampleQulity-1;
	d3dp.MultiSampleType=D3DMULTISAMPLE_8_SAMPLES;		//SwapEffect��ΪD3DSWAPEFFECT_DISCARDʱ���ò�����������ΪD3DMULTISAMPLE_NONE
	d3dp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	//d3dp.SwapEffect=D3DSWAPEFFECT_COPY;
	d3dp.Windowed=TRUE;

	if (mD9Device)
	{
		mIsLosing=false;
		HRESULT hr=mD9Device->Reset(&d3dp);
		switch (hr)
		{
		case D3DERR_DEVICELOST:
			{
				mIsLosing=true;
			}
			break;
		case D3D_OK:
			{
				ResetRenderState();
			}
			break;
		default:
			break;
		}
	}
	else
	{
		hr=mD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			//D3DDEVTYPE_REF,
			D3DDEVTYPE_HAL,
			mhWnd,
			dwBehavior|D3DCREATE_MULTITHREADED,
			&d3dp,
			&mD9Device
			);

		DWORD dwi=GetLastError();

		if (FAILED(hr))
		{
			return false;
		}
		SetLight();
	}

	return true;
}

void GDevice_D3D::setMaterial( D3DMATERIAL9* mat )
{
	mD9Device->SetMaterial(mat);
}
GDeviceType GDevice_D3D::getType()
{
	return GDeviceType_DX9;
}




