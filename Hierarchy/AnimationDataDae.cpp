#include "AnimationDataDae.h"

#include <iostream>
#include <String>
#include <fstream>
#include <vector>
#include <algorithm> // need for std::replace that erase uses
#include <sstream> // need for string stream

void AnimationDataDae::readDaeFile()
{
	const char* filePath = "Resources/Robot/MayaFiles/RobotIdleAnim.dae";
	std::stringstream textLineFromFile;
	bone newBone;
	int count;
	XMFLOAT3 splitTransform;
	std::string transOrRot;
	std::string textFromDaeFile;
	tinyxml2::XMLDocument daeFile;
	tinyxml2::XMLError loadFailed = daeFile.LoadFile(filePath);

	if (!loadFailed)
	{
		tinyxml2::XMLElement *parent = daeFile.FirstChildElement("COLLADA")->FirstChildElement("library_animations");
		for (tinyxml2::XMLElement *child = parent->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			textLineFromFile = std::stringstream(child->Attribute("id"));
			getline(textLineFromFile,textFromDaeFile, '.');
			if (newBone.boneName != textFromDaeFile)
			{
				bone newBone;
			}
			newBone.boneName = textFromDaeFile;
			getline(textLineFromFile, transOrRot, '.'); //lets us know if it's translation or toation may need to change though
			int loopCycle = 1;
			for (tinyxml2::XMLElement *floatArray = child->FirstChildElement(); floatArray != NULL; floatArray = floatArray->NextSiblingElement())
			{
				//we only want the input and output array, this is done in two loops if we look at the DAE file, we dont need the stuff after it.
				if (loopCycle == 1 || loopCycle == 2)
				{
					textFromDaeFile = std::string(floatArray->FirstChildElement()->Attribute("count"));
					count = std::stoi(textFromDaeFile);
					textLineFromFile = std::stringstream(floatArray->FirstChildElement("float_array")->GetText());
					if (transOrRot == "translate")
					{
						for (int i = 0; i < count; i++)
								{
									getline(textLineFromFile, textFromDaeFile, ' ');
									if (loopCycle == 1) //one means input which is time and two means output which is xyz co-ords
									{
										newBone.tranTime.push_back(std::stof(textFromDaeFile));
									}
									else {
										splitTransform.x = std::stof(textFromDaeFile);
										getline(textLineFromFile, textFromDaeFile, ' ');
										i++;
										splitTransform.y = std::stof(textFromDaeFile);
										getline(textLineFromFile, textFromDaeFile, ' ');
										i++;
										splitTransform.z = std::stof(textFromDaeFile);
										
										newBone.translate.push_back(splitTransform);
										
									}
									
								}
					} 
					else if (transOrRot == "rotateX")
					{
						for (int i = 0; i < count; i++)
						{
							getline(textLineFromFile, textFromDaeFile, ' ');
							if (loopCycle == 1)
							{
								newBone.rotTime.push_back(std::stof(textFromDaeFile));
							}
							else {
								newBone.rotX.push_back(std::stof(textFromDaeFile));

							}//may need to change newbone from a struct to a class

						}
					}
					else if (transOrRot == "rotateY")
					{
						for (int i = 0; i < count; i++)
						{
							getline(textLineFromFile, textFromDaeFile, ' ');
							if (loopCycle == 2) 
							{
								newBone.rotY.push_back(std::stof(textFromDaeFile));
							}
						}
					}
					else if (transOrRot == "rotateZ")
					{
						for (int i = 0; i < count; i++)
						{
							getline(textLineFromFile, textFromDaeFile, ' ');
							if (loopCycle == 2)
							{
								newBone.rotZ.push_back(std::stof(textFromDaeFile));
							}
						}
					}
					
				}
				else {
					break;
				}
				loopCycle++;
			}

			//textFromDaeFile = std::string(child->FirstChildElement()->FirstChildElement("float_array")->Attribute("count"));
			//int count = std::stoi(textFromDaeFile);
			//tinyxml2::XMLElement *floatArray = child->FirstChildElement("source")->FirstChildElement("float_array");
			//textLineFromFile = std::stringstream(child->FirstChildElement()->FirstChildElement("float_array")->GetText());
			//tinyxml2::XMLText* textNode = child->FirstChildElement()->FirstChildElement("float_array")->GetText();

			//if (transOrRot == "translate")
			//{
			//	for (int i = 0; i < count; i++)
			//	{
			//		getline(textLineFromFile, textFromDaeFile, ' ');
			//		newBone.tranTime.push_back(std::stof(textFromDaeFile));
			//	}
			//}

			
			boneAnimation.push_back(newBone);
		}
		boneAnimation.push_back(newBone);
	}
	else 
	{

	}

}
