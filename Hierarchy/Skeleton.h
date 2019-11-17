#ifndef SKELETON_H
#define SKELETON_H

#include "Robot.h"
#include "Transform.h"


__declspec(align(16)) class Skeleton
{
public:
	Skeleton();
	~Skeleton(void);


	//void LoadResource(std::string foldername); // Only load the resources once for all instances
	//void ReleaseResource(void); // Only free the resources once for all instances


	//void Update(bool bPlayerControl); // Player only has control of plane when flag is set
	//void Draw(void);

	//CommonMesh* s_pSkeletonBodyPartMesh; //Only need one body party mesh
	XMFLOAT4 m_v4SkeletonRotation; // Euler rotation angles
	XMFLOAT4 m_v4SkeletonPositionOffset; // offset
	XMMATRIX m_mSkeletonLocalMatrix = XMMatrixIdentity();
	XMMATRIX m_mSkeletonWorldMatrix = XMMatrixIdentity();
	//Transform transform;
	std::string partName;
	

private:
	

	

public:
	float GetXPosition(void) { return m_v4SkeletonPositionOffset.x; }
	float GetYPosition(void) { return m_v4SkeletonPositionOffset.y; }
	float GetZPosition(void) { return m_v4SkeletonPositionOffset.z; }

	XMFLOAT4 GetPosition(void) { return m_v4SkeletonPositionOffset; }

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

};
#endif

