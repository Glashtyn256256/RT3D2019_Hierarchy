#ifndef ROBOT_H
#define ROBOT_H

#include "Application.h"
#include <iostream>
#include <String>
#include <fstream>
#include <vector>
#include "Skeleton.h"
#include <algorithm> // need for std::replace that erase uses
#include <sstream> // need for string stream
#include "Transform.h"

class Robot
{
public:
	Robot(std::string filePath);
	~Robot(void);

	void LoadResources(void);
	void ReleaseResources(void);

	void UpdateMatrices(void);
	void Update();
	void DrawAll(void);
	Transform transform;
	
	//void SetWorldPosition(float fX, float fY, float fZ);
private:
	Skeleton* skeleton;
	std::vector<Skeleton> skeletonParts;
	//std::vector<std::string> skeletonPartsParent;
	//Instead of the string I've changed it to the position in the array, saves me looping through for loops.
	std::vector<int> skeletonPartsParent; 
	std::string folderName;
	XMFLOAT4 m_v4WorldPosition;
	std::vector<CommonMesh*> meshCollection;
};
#endif
