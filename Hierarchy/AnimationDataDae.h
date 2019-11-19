#ifndef ANIMATION_DATA_DAE_H
#define ANIMATION_DATA_DAE_H

#include "Application.h"

struct bone {
	std::string boneName; //getboneName
	int keyframeCount;
	std::vector<float> time;
	std::vector<XMFLOAT3> translate;
	std::vector<XMFLOAT3> rotation;
};

class AnimationDataDae 
{
public:
	
	std::vector<bone> boneAnimation;
	void readDaeFile();
private:

};




#endif