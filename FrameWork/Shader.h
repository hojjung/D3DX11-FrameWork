#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h> 

using namespace DirectX;

class C_SHADER
{
private:
	ID3D11VertexShader*     m_pVertexShader;
	ID3D11InputLayout*      m_pVertexLayout;

	ID3D11PixelShader*      m_pPixelShader;
	ID3D11Buffer*           m_pConstantBuffer;


private:
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	C_SHADER();
	HRESULT create(ID3D11Device* pD3dDevice  , const WCHAR * szFileName,
		const D3D11_INPUT_ELEMENT_DESC *pLayOut , int nLayOutSize , int nConstBufferSize);

	ID3D11VertexShader*		getVS();
	ID3D11PixelShader*		getPS();
	ID3D11Buffer * const *	getCB();
	void					UpdateSubresource(ID3D11DeviceContext*  pDeviceContext, const void * pResource);
	ID3D11InputLayout*		getInputLayout();
	void					Release();

};