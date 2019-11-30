//*********************************************************************************************
// File:			Aeroplane.cpp
// Description:		A very simple class to represent an aeroplane as one object with all the
//					hierarchical components stored internally within the class.
// Module:			Real-Time 3D Techniques for Games
// Created:			Jake - 2010-2011
// Notes:
//*********************************************************************************************

#include "Aeroplane.h"

// Initialise static class variables.
CommonMesh* Aeroplane::s_pPlaneMesh = NULL;
CommonMesh* Aeroplane::s_pPropMesh = NULL;
CommonMesh* Aeroplane::s_pTurretMesh = NULL;
CommonMesh* Aeroplane::s_pGunMesh = NULL;
CommonMesh* Aeroplane::pBulletMesh = NULL;

bool Aeroplane::s_bResourcesReady = false;

Aeroplane::Aeroplane(float fX, float fY, float fZ, float fRotY)
{
	m_mWorldMatrix = XMMatrixIdentity();
	m_mPropWorldMatrix = XMMatrixIdentity();
	m_mTurretWorldMatrix = XMMatrixIdentity();
	m_mGunWorldMatrix = XMMatrixIdentity();
	m_mCamWorldMatrix = XMMatrixIdentity();

	m_v4Rot = XMFLOAT4(0.0f, fRotY, 0.0f, 0.0f);
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);

	m_v4PropOff = XMFLOAT4(0.0f, 0.0f, 1.9f, 0.0f);
	m_v4PropRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4TurretOff = XMFLOAT4(0.0f, 1.05f, -1.3f, 0.0f);
	m_v4TurretRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4GunOff = XMFLOAT4(0.0f, 0.5f, 0.0f, 0.0f);
	m_v4GunRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_v4BulletOff = XMFLOAT4(0.0f, 0.0f, 1.2f, 0.0f);
	m_v4BulletRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_v4BulletScale = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);

	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_fSpeed = 0.0f;
	test = 0;
	m_bGunCam = false;
}

Aeroplane::~Aeroplane(void)
{
	//delete newBullet;
}
Aeroplane::GunBullet::GunBullet(XMMATRIX bulletworldposition)
{
	XMMATRIX  mScale, mTran;

	bulletOffset = {0.0f, 0.0f, 1.4f, 0.0f}; 
	bulletRotation = { 0.0f,0.0f,0.0f,0.0f };
	bulletScale = { 0.1f, 0.1f, 0.1f, 0.0f };
	mTran = XMMatrixTranslationFromVector(XMLoadFloat4(&bulletOffset));
	mScale = XMMatrixScalingFromVector(XMLoadFloat4(&bulletScale));
	bulletWorldPosition = mScale * mTran * bulletworldposition;
	survivalTime = 0.0f;
	speedBullet = 4.0f;
}

void Aeroplane::SetWorldPosition(float fX, float fY, float fZ)
{
	m_v4Pos = XMFLOAT4(fX, fY, fZ, 0.0f);
	UpdateMatrices();
}

void Aeroplane::UpdateMatrices(void)
{
	XMMATRIX mRotX, mRotY, mRotZ, mTrans, mScale;
	XMMATRIX mPlaneCameraRot, mForwardMatrix;


	// [START HERE]
	// x y z
	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4Pos));
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4Rot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4Rot.z));
	m_mWorldMatrix = mRotZ * mRotX * mRotY * mTrans;
	

	// Calculate m_mWorldMatrix for plane based on Euler rotation angles and position data.
	// i.e. Use XMMatrixRotationX(), XMMatrixRotationY(), XMMatrixRotationZ() and XMMatrixTranslationFromVector to calculate mRotX, mRotY, mRotZ and mTrans from m_v4Rot
	// Then concatenate the matrices to calculate m_mWorldMatrix

	// [Skip this step first time through] Also calculate mPlaneCameraRot which ignores rotations in Z and X for the camera to parent to
	mPlaneCameraRot = mRotY * mTrans;
	// [Skip this step first time through] Get the forward vector out of the world matrix and put it in m_vForwardVector
	m_vForwardVector = m_mWorldMatrix.r[2]; //this is z, which is why we go forward
	// Calculate m_mPropWorldMatrix for propeller based on Euler rotation angles and position data.
	// Parent the propeller to the plane

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4PropOff));
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4PropRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4PropRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4PropRot.z));
	XMMATRIX m_mLocalProp = mRotZ * mRotX *  mRotY * mTrans;
	m_mPropWorldMatrix = m_mLocalProp * m_mWorldMatrix;

	// Calculate m_mTurretWorldMatrix for propeller based on Euler rotation angles and position data.
	// Parent the turret to the plane

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4TurretOff));
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4TurretRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4TurretRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4TurretRot.z));
	XMMATRIX m_mLocalTurret = mRotZ * mRotX *  mRotY * mTrans;
	m_mTurretWorldMatrix = m_mLocalTurret * m_mWorldMatrix;
	// Calculate m_mGunWorldMatrix for gun based on Euler rotation angles and position data.
	// Parent the gun to the turret

	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4GunOff));
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4GunRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4GunRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4GunRot.z));
	
	XMMATRIX m_mLocalGun = mRotZ * mRotX *  mRotY * mTrans;
	m_mGunWorldMatrix = m_mLocalGun * m_mTurretWorldMatrix;

	// Calculate m_mCameraWorldMatrix for camera based on Euler rotation angles and position data.
	mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&m_v4CamOff));
	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4CamRot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4CamRot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4CamRot.z));
	XMMATRIX m_mLocalCam = mRotZ * mRotX *  mRotY * mTrans;
	//m_mCamWorldMatrix = m_mLocalCam * m_mWorldMatrix ;


	// [Skip this step first time through] Switch between parenting the camera to the plane (without X and Z rotations) and the gun based on m_bGunCam

	if (m_bGunCam)
	{
		m_mCamWorldMatrix = m_mLocalCam * m_mGunWorldMatrix;
	}
	else {
		m_mCamWorldMatrix = m_mLocalCam * mPlaneCameraRot;
	}

	// Get the camera's world position (m_vCamWorldPos) out of m_mCameraWorldMatrix
	m_vCamWorldPos = m_mCamWorldMatrix.r[3];
	
	if (bulletContainer.size() > 0)
	{
		for (int i = 0; i < bulletContainer.size(); i++)
		{

			mTrans = XMMatrixTranslationFromVector(XMLoadFloat4(&bulletContainer[i]->bulletOffset));
			bulletContainer[i]->bulletWorldPosition =  mTrans * bulletContainer[i]->bulletWorldPosition;
		}
	}
	
	
}

void Aeroplane::deleteBullet()
{
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		if (bulletContainer[i]->survivalTime > 5.0f)
		{

			delete(bulletContainer[i]); //delete the pointer stop memory leaks
			bulletContainer.erase(bulletContainer.begin()); //remove the empty vector position
			--i; //Go back to the start of the vector
		}
		
	}
}

void Aeroplane::Update(bool bPlayerControl)
{
	deleteBullet();
	
	// DON'T DO THIS UNTIL YOu HAVE COMPLETED THE FUNCTION ABOVE
	if(bPlayerControl)
	{
		// Step 1: Make the plane pitch upwards when you press "Q" and return to level when released
		// Maximum pitch = 60 degrees
		if (Application::s_pApp->IsKeyPressed('Q')) 
		{
			if (m_v4Rot.x <= 60.f)
			{
				m_v4Rot.x += 1.f;
			}
		}
		else
		{
			if (m_v4Rot.x > 0)
			{
				m_v4Rot.x -= 1.f;
			}
		}
		// Step 2: Make the plane pitch downwards when you press "A" and return to level when released
		// You can also impose a take off speed of 0.5 if you like
		// Minimum pitch = -60 degrees

		if (Application::s_pApp->IsKeyPressed('A'))
		{
			if (m_v4Rot.x >= -60.f)
			{
				m_v4Rot.x -= 1.f;
			}
		}
		else
		{
			if (m_v4Rot.x < 0)
			{
				m_v4Rot.x += 1.f;
			}
		}

		// Step 3: Make the plane yaw and roll left when you press "O" and return to level when released
		// Maximum roll = 20 degrees
		if (Application::s_pApp->IsKeyPressed('O'))
		{
			if ((m_v4Rot.z <= 20.f))
			{
				m_v4Rot.z += 1.f; //roll
			}
			m_v4Rot.y -= 1.f; //yaw
		}
		else
		{
			if (m_v4Rot.z > 0)
			{
				m_v4Rot.z -= 1.f; //roll
			}
		}
		// Step 4: Make the plane yaw and roll right when you press "P" and return to level when released
		// Minimum roll = -20 degrees
		if (Application::s_pApp->IsKeyPressed('P'))
		{
			if ((m_v4Rot.z >= -20.f))
			{
				m_v4Rot.z -= 1.f; //roll
				//m_v4Rot.y -= +1.f; //yaw
			}
			m_v4Rot.y += 1.f; //yaw

		}
		else
		{
			if (m_v4Rot.z < 0)
			{
				m_v4Rot.z += 1.f; //roll
			}
		}

	

	} // End of if player control
	if (Application::s_pApp->IsKeyPressed(VK_SPACE))
	{
		//instantiate a new bullet
		newBullet = new GunBullet(m_mGunWorldMatrix);
		//add it to the vector of bullets
		bulletContainer.push_back(newBullet);
	}
	
	// Apply a forward thrust and limit to a maximum speed of 1

	

	m_fSpeed += 0.001f;

	if(m_fSpeed > 1)
		m_fSpeed = 1.0f;

	if (bulletContainer.size() > 0)
	{
		for (int i = 0; i < bulletContainer.size(); i++)
		{
			bulletContainer[i]->survivalTime += 1.0 / 60.0f;
			bulletContainer[i]->bulletOffset.z += bulletContainer[i]->speedBullet + m_fSpeed;
		}
	}

	
	// Rotate propeller and turret
	m_v4PropRot.z += 100 * m_fSpeed;
	//m_v4TurretRot.y += 0.1f;
	//m_v4Rot.y += 10 ; 
	// Tilt gun up and down as turret rotates
	//m_v4GunRot.x = (sin((float)XMConvertToRadians(m_v4TurretRot.y * 4.0f)) * 10.0f) - 10.0f;

	UpdateMatrices();

	// Move Forward
	XMVECTOR vCurrPos = XMLoadFloat4(&m_v4Pos);
	vCurrPos += m_vForwardVector * m_fSpeed;
	XMStoreFloat4(&m_v4Pos, vCurrPos);
}

void Aeroplane::LoadResources(void)
{
	s_pPlaneMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/plane.x");
	s_pPropMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/prop.x");
	s_pTurretMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/turret.x");
	s_pGunMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/gun.x");
	pBulletMesh = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/bullet.x");
}

void Aeroplane::ReleaseResources(void)
{
	delete s_pPlaneMesh;
	delete s_pPropMesh;
	delete s_pTurretMesh;
	delete s_pGunMesh;
	delete pBulletMesh;
}

void Aeroplane::Draw(void)
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	s_pPlaneMesh->Draw();
	
	Application::s_pApp->SetWorldMatrix(m_mPropWorldMatrix);
	s_pPropMesh->Draw();
	
	Application::s_pApp->SetWorldMatrix(m_mTurretWorldMatrix);
	s_pTurretMesh->Draw();

	Application::s_pApp->SetWorldMatrix(m_mGunWorldMatrix);
	s_pGunMesh->Draw();


	/*Application::s_pApp->SetWorldMatrix(m_mBulletWorldMatrix);
	s_pBulletMesh->Draw();*/
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		Application::s_pApp->SetWorldMatrix(bulletContainer[i]->bulletWorldPosition);
		pBulletMesh->Draw();
	}
}
