#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h>

using namespace DirectX;

class C_RENDER_DATA
{
private:
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
	};

private:
	ID3D11Buffer*           m_pVertexBuffer;
	ID3D11Buffer*           m_pIndexBuffer;

private:

	HRESULT createD3D11Buffer(ID3D11Device* pD3dDevice , const void *pData, int nDataSize, UINT nBindFlag, ID3D11Buffer **ppBuffer);

public:
	C_RENDER_DATA();
	bool create(ID3D11Device* pD3dDevice , ID3D11DeviceContext * pImmediateContext);
	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	void Release();
};