#include "stdafx.h"

C_SHADER::C_SHADER()
{
}

HRESULT C_SHADER::CompileShaderFromFile(const WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT C_SHADER::create(ID3D11Device * pD3dDevice, const WCHAR * szFileName,
	const D3D11_INPUT_ELEMENT_DESC *pLayOut, int nLayOutSize , int nConstBufferSize)
{
	HRESULT hr = S_FALSE;
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(szFileName, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	hr = pD3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	hr = pD3dDevice->CreateInputLayout(pLayOut, nLayOutSize, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = nConstBufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pD3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
		return hr;

	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(szFileName, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pD3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}

ID3D11VertexShader * C_SHADER::getVS()
{
	return m_pVertexShader;
}

ID3D11PixelShader * C_SHADER::getPS()
{
	return m_pPixelShader;
}

ID3D11Buffer * const *	C_SHADER::getCB()
{
	return &m_pConstantBuffer;
}

void C_SHADER::UpdateSubresource(ID3D11DeviceContext*  pDeviceContext , const void * pResource)
{
	pDeviceContext->UpdateSubresource(m_pConstantBuffer , 0, NULL, pResource, 0, 0);
}

ID3D11InputLayout * C_SHADER::getInputLayout()
{
	return m_pVertexLayout;
}

void C_SHADER::Release()
{
	if (m_pVertexShader)
		m_pVertexShader->Release();

	if(m_pConstantBuffer)
		m_pConstantBuffer->Release();

	if (m_pVertexLayout)
		m_pVertexLayout->Release();

	if (m_pPixelShader)
		m_pPixelShader->Release();

}

