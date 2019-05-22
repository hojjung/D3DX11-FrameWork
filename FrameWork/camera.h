#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h>

using namespace DirectX;

class C_CAMERA
{
private:
	XMVECTOR		m_vPosition;

	float			m_fPitch;
	float			m_fYaw;
	float			m_fRoll;
	XMMATRIX		m_matView;
	XMMATRIX		m_matRotation;
	float			m_fV;
	XMVECTOR		m_vForwardDir;
	XMVECTOR		m_vRighitDir;
	static const XMVECTOR	m_vZDir;
	static const XMVECTOR	m_vXDir;

private:
	void radianInterpolation(float &fRid);

public:
	C_CAMERA();
	const XMMATRIX* getMatrixView();
	void rotateYawPitchRoll(float fYaw, float fPitch, float fRoll);
	void moveF();
	void moveB();
	void moveL();
	void moveR();
	void update();
	void setVelocity(float fV);
	void setPosition(float fX, float fY, float fZ);

	void * operator new (size_t stAllocateBlock);
	void operator delete(void * pMemoryBlock);
};