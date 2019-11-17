#include "Transform.h"

XMMATRIX Transform::CalculateLocalMatrix(const XMFLOAT4& skeletonpos, const XMFLOAT4& skeletonrot)
{
	XMMATRIX mRotX, mRotY, mRotZ, mTrans, m_mLocalMatrix;

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&skeletonpos));
	mRotX = XMMatrixRotationX(XMConvertToRadians(skeletonrot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(skeletonrot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(skeletonrot.z));

	return XMMATRIX(mRotZ * mRotX * mRotY * mTrans);
}

XMMATRIX Transform::CalculateWorldMatrix(const XMMATRIX& m_mskeletonlocal, const XMMATRIX& m_mskeletonparent, const XMMATRIX& m_mskeletonworld)
{
	XMMATRIX m_mWorldMatrix;

	return m_mWorldMatrix = m_mskeletonlocal * m_mskeletonparent * m_mskeletonworld;
}

XMMATRIX Transform::CalculateWorldMatrix(const XMMATRIX& m_mskeletonlocal, const XMMATRIX& m_mskeletonworld )
{
	XMMATRIX m_mWorldMatrix;

	return m_mWorldMatrix = m_mskeletonlocal * m_mskeletonworld;
}


//need to work out parents of the parents of the parents I think?
//example wrist * arm * shoulder * body * world