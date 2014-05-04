#pragma once


typedef D3DXHANDLE(*SetPara)(ID3DXEffect *);

D3DXHANDLE SetEffectParaments(ID3DXEffect *pEffect);

class CXEffect
{
public:
	CXEffect(void);
	~CXEffect(void);

	bool Create(char *sEffectFileName);

	D3DXHANDLE SetEffectPara(SetPara FunSetPara);

public:

	ID3DXEffect *mD3DEffect;

	ID3DXEffectPool *m_pEffectPool;

	D3DXHANDLE m_hParaBlock;

	D3DXHANDLE m_hWorldViewProj;

	D3DXHANDLE m_Tech;

	D3DXHANDLE m_hUseMaterialOnly;

	D3DXHANDLE m_hTexture;

	D3DXHANDLE m_hMtrlAmbient;

	D3DXHANDLE m_hMtrlDiffuse;

	D3DXHANDLE m_hWorld;

	D3DXHANDLE m_hOpenLight;


	IDirect3DVertexDeclaration9 *m_pVDel;

public:

};

extern CXEffect gEffect;