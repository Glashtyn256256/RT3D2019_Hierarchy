#ifndef ANIMATION_DATA_DAE_H
#define ANIMATION_DATA_DAE_H

#include "Application.h"
#include "tinyxml2.h"

struct bone {
	std::string boneName; //getboneName

	std::vector <float> tranTime;
	std::vector<float> rotTime;
	std::vector<XMFLOAT3> translate;
	std::vector<float> rotX;
	std::vector<float> rotY;
	std::vector<float> rotZ;
	int tranKeyframeCount;
	int rotKeyframeCount;
};

class AnimationDataDae 
{
public:
	
	std::vector<bone> boneAnimation;
	void readDaeFile();
private:

};




#endif