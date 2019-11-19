#include "AnimationDataDae.h"
#include <String>
#include <fstream>
#include <vector>

void AnimationDataDae::readDaeFile()
{
	std::string textLineFromFile;
	std::fstream textFile("Resources/Robot/RobotIdleAnimDAE.txt");

	std::getline(textFile, textLineFromFile);
	while (!textFile.eof())
	{
		bone newBone;

		std::getline(textFile, textLineFromFile);
		textLineFromFile = textLineFromFile.substr(textLineFromFile.find('\"'), textLineFromFile.find('.'));
		newBone.boneName = textLineFromFile;

		boneAnimation.push_back(newBone);
	}

}
