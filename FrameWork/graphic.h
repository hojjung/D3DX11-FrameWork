#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "RenderData.h"
#include "Shader.h"

using namespace DirectX;

class C_GRAPHIC
{
private:

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
		XMFLOAT4 vLightDir[2];
		XMFLOAT4 vLightColor[2];
		XMFLOAT4 vOutputColor;
	};

	C_RENDER_DATA			m_cRenderData;
	C_SHADER				m_cShader;
	C_SHADER				m_cShaderSolid;

	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	ID3D11Device*           g_pd3dDevice;
	ID3D11DeviceContext*    g_pImmediateContext;
	IDXGISwapChain*         g_pSwapChain;
	ID3D11RenderTargetView* g_pRenderTargetView;
	ID3D11Texture2D*        g_pDepthStencil;
	ID3D11DepthStencilView* g_pDepthStencilView;
	XMMATRIX                g_World;
	//XMMATRIX                g_View;
	XMMATRIX                g_Projection;
	
private:
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
public:
	C_GRAPHIC();
	HRESULT InitDevice(HWND hWnd);
	void CleanupDevice();
	void Render(const XMMATRIX *pMatView);

	void * operator new (size_t stAllocateBlock);
	void operator delete(void * pMemoryBlock);
};