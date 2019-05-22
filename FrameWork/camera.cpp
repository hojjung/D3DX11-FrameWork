#include "stdafx.h"

const XMVECTOR	C_CAMERA::m_vZDir(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
const XMVECTOR	C_CAMERA::m_vXDir(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));


C_CAMERA::C_CAMERA() :
	m_vPosition{},
	m_fYaw{},
	m_fPitch{},
	m_matView{},
	m_fRoll{},
	m_matRotation{},
	m_fV{},
	m_vForwardDir{},
	m_vRighitDir{}
{
	m_matView = XMMatrixIdentity();
	m_matRotation = XMMatrixIdentity();
	m_vForwardDir = m_vZDir;
	m_vRighitDir = m_vXDir;
	m_fV = 0.01f;
}

const XMMATRIX * C_CAMERA::getMatrixView()
{
	return &m_matView;
}

void C_CAMERA::radianInterpolation(float &fRid)
{
	if (fRid < 0.0f)
		fRid += XM_2PI;
	else if (fRid > XM_2PI)
		fRid -= XM_2PI;
}

void C_CAMERA::rotateYawPitchRoll(float fYaw, float fPitch, float fRoll)
{
	m_fYaw += fYaw;
	m_fPitch += fPitch;
	m_fRoll += fRoll;

	radianInterpolation(m_fYaw);
	radianInterpolation(m_fPitch);
	radianInterpolation(m_fRoll);

	m_matRotation = XMMatrixRotationRollPitchYaw(m_fPitch, m_fYaw, m_fRoll);
	m_vForwardDir = XMVector3TransformCoord(m_vZDir, m_matRotation);
	m_vRighitDir = XMVector3TransformCoord(m_vXDir, m_matRotation);
}

void C_CAMERA::moveF()
{
	m_vPosition += m_vForwardDir * m_fV;
}

void C_CAMERA::moveB()
{
	m_vPosition += m_vForwardDir * -m_fV;
}

void C_CAMERA::moveL()
{
	m_vPosition += m_vRighitDir * -m_fV;
}

void C_CAMERA::moveR()
{
	m_vPosition += m_vRighitDir * m_fV;
}

void C_CAMERA::update()
{
	m_matView = m_matRotation * XMMatrixTranslationFromVector(m_vPosition);

	m_matView = XMMatrixInverse(nullptr, m_matView);
}

void C_CAMERA::setVelocity(float fV)
{
	m_fV = fV;
}

void C_CAMERA::setPosition(float fX, float fY, float fZ)
{
	m_vPosition = XMVectorSet(fX, fY, fZ, 0.0f);
}

void * C_CAMERA::operator new(size_t stAllocateBlock)
{
	return _aligned_malloc(sizeof(C_GRAPHIC), 16);
}

void C_CAMERA::operator delete(void *pMemoryBlock)
{
	_aligned_free(pMemoryBlock);
}