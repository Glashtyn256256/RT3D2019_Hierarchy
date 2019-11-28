#include "Robot.h"
#include <math.h>

//Robot::Robot(std::string filepath)
//{
//	std::string textLineFromFile;
//	std::fstream textFile (filepath);
//
//	std::getline(textFile, textLineFromFile);
//	std::getline(textFile, textLineFromFile);
//	textLineFromFile.erase(std::remove(textLineFromFile.begin(),
//		textLineFromFile.end(), '\"'),
//		textLineFromFile.end());
//	folderName = textLineFromFile;
//	
//		while (!textFile.eof())
//		{
//			skeleton = new Skeleton();
//			std::getline(textFile, textLineFromFile);
//			//textLineFromFile.erase(std::find(textLineFromFile.begin(), textLineFromFile.end(),'\"'));
//			//textLineFromFile.replace(textLineFromFile.begin(), textLineFromFile.end(), "\"", "");
//			textLineFromFile.erase(std::remove(textLineFromFile.begin(), 
//				textLineFromFile.end(), '\"'), 
//				textLineFromFile.end());
//			skeleton->partName = textLineFromFile;
//
//			std::getline(textFile, textLineFromFile);
//			textLineFromFile.erase(std::remove(textLineFromFile.begin(),
//				textLineFromFile.end(), '\"'),
//				textLineFromFile.end());
//			if (textLineFromFile != "")
//			{
//				skeletonPartsParent.push_back(textLineFromFile);
//			}
//
//			getline(textFile, textLineFromFile);
//			std::vector<float> splitFloat;
//			std::stringstream splitString(textLineFromFile); 
//			
//			for (size_t i = 0; i < 3; i++)
//			{	
//				getline(splitString, textLineFromFile, ',');
//				splitFloat.push_back(std::stof(textLineFromFile));
//			}
//
//			skeleton->m_v4SkeletonPositionOffset.x = splitFloat[0];
//			skeleton->m_v4SkeletonPositionOffset.y = splitFloat[1];
//			skeleton->m_v4SkeletonPositionOffset.z = splitFloat[2];
//			skeleton->m_v4SkeletonPositionOffset.w = {0.f};
//
//			splitFloat.clear();
//
//			skeleton->m_v4SkeletonRotation = { 0.0f, 0.0f, 0.0f, 0.0f };
//
//			skeletonParts.push_back(*skeleton);
//			delete skeleton;
//	}

//}

Robot::Robot(std::string filepath, float wPosX, float wPosY, float wPosZ, float wPosW, float wRotX, float wRotY, float wRotZ, float wRotW)
{
	SetWorldPosition(wPosX, wPosY, wPosZ, wPosW, wRotX, wRotY, wRotZ, wRotW);
	ReadTextFileAndSetUpModel(filepath);
	SetUpMeshes();
	SetUpAnimations();
}

void Robot::ReadTextFileAndSetUpModel(std::string filepath)
{
	std::string textLineFromFile;
	std::fstream textFile(filepath);

	std::getline(textFile, textLineFromFile);
	textLineFromFile.erase(std::remove(textLineFromFile.begin(),
		textLineFromFile.end(), '\"'),
		textLineFromFile.end());
	folderName = textLineFromFile;

	while (!textFile.eof())
	{
		skeleton = new Skeleton();
		std::getline(textFile, textLineFromFile);
		//textLineFromFile.erase(std::find(textLineFromFile.begin(), textLineFromFile.end(),'\"'));
		//textLineFromFile.replace(textLineFromFile.begin(), textLineFromFile.end(), "\"", "");
		textLineFromFile.erase(std::remove(textLineFromFile.begin(),
			textLineFromFile.end(), '\"'),
			textLineFromFile.end());
		skeleton->SetPartName(textLineFromFile);

		std::getline(textFile, textLineFromFile);
		skeletonPartsParent.push_back(std::stoi(textLineFromFile));

		getline(textFile, textLineFromFile);
		std::vector<float> splitFloat;
		std::stringstream splitString(textLineFromFile);

		for (size_t i = 0; i < 3; i++)
		{
			getline(splitString, textLineFromFile, ',');
			splitFloat.push_back(std::stof(textLineFromFile));
		}
		skeleton->SetSkeletonOffsetPosition(splitFloat[0] / 10, splitFloat[1] / 10, splitFloat[2] / 10, 0.f);
		splitFloat.clear();

		skeleton->SetSkeletonRotationPosition(0.0f, 0.0f, 0.0f, 0.0f);

		skeleton->SetLocalMatrix(); //now we have pos and rot we can set up the local matrix for the bone;
		skeletonParts.push_back(*skeleton);
		delete skeleton;
	}
}

void Robot::SetUpMeshes()
{
	for (int i = 1; i < skeletonParts.size(); i++) //Start at one because you don't need a mesh for the root
	{
		meshCollection.push_back(nullptr);
	}
};

void Robot::SetUpAnimations(void) {
	animationAttack = new AnimationDataDae("Resources/Robot/MayaFiles/RobotAttackAnim.dae");
	animationIdle = new AnimationDataDae("Resources/Robot/MayaFiles/RobotIdleAnim.dae");
	animationDeath = new AnimationDataDae("Resources/Robot/MayaFiles/RobotDieAnim.dae");

	currentAnimation = nullptr;
	animTime = 0;
}

Robot::~Robot(void)
{
	delete animationAttack;
	delete animationIdle;
}

void Robot::UpdateMatrices(void)
{
	for (int i = 0; i < skeletonParts.size(); i++)
	{
		skeletonParts[i].SetLocalMatrix();

		if (skeletonParts[i].GetPartName() == "root")
		{
			skeletonParts[i].SetWorldMatrix(m_mWorldPosition);
		}
		else
		{
			skeletonParts[i].SetWorldMatrix(skeletonParts[skeletonPartsParent[i] - 1].GetWorldMatrix());
		}
	}
}

void Robot::Update()
{
	if (Application::s_pApp->IsKeyPressed('1'))
	{
		currentAnimation = animationAttack;
	}
	if (Application::s_pApp->IsKeyPressed('2'))
	{
		currentAnimation = animationIdle;
	}
	if (Application::s_pApp->IsKeyPressed('3'))
	{
		currentAnimation = animationDeath;
	}

	if (currentAnimation)
	{

		//debug helper delete when done
		if (count == 86) {
			count++;
		}

		//deltatime
		animTime += 1.0f / 180;
		for (int i = 0; i < skeletonParts.size(); i++)
		{

			//Gets the correct animation thanks to the naming
			SkeletonAnimationData* data = currentAnimation->boneAnimation[skeletonParts[i].GetPartName()];
			if (data)
			{
				Skeleton& bone = skeletonParts[i];

				if (animTime >= currentAnimation->endTime)
				{
					data->rotCurrentFrame = 0;
					data->tranCurrentFrame = 0;
				}
					int test(data->tranTime.size() - 1);
					int currentTranFrame = data->tranCurrentFrame;
					if (!(currentTranFrame > test))
					{
						float translationEndTime = data->tranTime[currentTranFrame];
						XMFLOAT4 previousTranslationF4;

						if (currentTranFrame == 0)
						{
							previousTranslationF4 = bone.GetOffsetPosition();
						}
						else
						{
							previousTranslationF4 = data->translate[currentTranFrame - 1];
						}

						float tX = data->translate[currentTranFrame].x;
						float tY = data->translate[currentTranFrame].y;
						float tZ = data->translate[currentTranFrame].z;
						float tW = data->translate[currentTranFrame].w;
						if (!(animTime >= translationEndTime))
						{
							float tLerp = (animTime - data->previousTranslationTime) / (translationEndTime - data->previousTranslationTime);
							if (!(std::isinf(tLerp)))
							{
								XMVECTOR startTranslation = XMVectorSet(previousTranslationF4.x, previousTranslationF4.y, previousTranslationF4.z, previousTranslationF4.w);
								XMVECTOR previousTranslation = XMVectorSet(tX, tY, tZ, tW);
								XMVECTOR newTranslation = XMVectorLerp(startTranslation, previousTranslation, tLerp);
								XMFLOAT4 newTranslationF4;
								XMStoreFloat4(&newTranslationF4, newTranslation);

								bone.SetSkeletonOffsetPosition(newTranslationF4.x, newTranslationF4.y, newTranslationF4.z, 0.0f);
							}
						}
						else
						{
							count++;
						}
						if (animTime >= translationEndTime)
						{
							data->previousTranslationTime = translationEndTime;

							currentTranFrame++;
							if (currentTranFrame > data->tranTime.size())
								currentTranFrame = data->tranTime.size();
						}
						data->tranCurrentFrame = currentTranFrame;
					}

					int test1(data->rotTime.size() - 1);
					int currentRotFrame = data->rotCurrentFrame;
					if (!(currentRotFrame > test1))
					{
						
						float rotationEndTime = data->rotTime[currentRotFrame];
						XMFLOAT4 previousRotationF4;

						if (currentRotFrame == 0)
						{
							previousRotationF4 = bone.GetRotationPosition();
						}
						else
						{
							float prX = data->rotX[currentRotFrame - 1];
							float prY = data->rotY[currentRotFrame - 1];
							float prZ = data->rotZ[currentRotFrame - 1];
							previousRotationF4 = XMFLOAT4(prX, prY, prZ, 0.0f);
						}
						if (!(animTime >= rotationEndTime))
						{
							float rLerp = (animTime - data->previousRotationTime) / (rotationEndTime - data->previousRotationTime);
							if (!(std::isinf(rLerp)))
							{
								float rX = data->rotX[currentRotFrame];
								float rY = data->rotY[currentRotFrame];
								float rZ = data->rotZ[currentRotFrame];

								XMVECTOR startRotation = XMVectorSet(previousRotationF4.x, previousRotationF4.y, previousRotationF4.z, 0.0f);
								XMVECTOR previousRotation = XMVectorSet(rX, rY, rZ, 0.0f);


								XMVECTOR newRotation = XMVectorLerp(startRotation, previousRotation, rLerp);
								XMFLOAT4 newRotationF4;
								XMStoreFloat4(&newRotationF4, newRotation);

								bone.SetSkeletonRotationPosition(newRotationF4.x, newRotationF4.y, newRotationF4.z, 0.0f);
							}
						}

						if (animTime >= rotationEndTime)
						{
							data->previousRotationTime = rotationEndTime;

							currentRotFrame++;
							if (currentRotFrame > data->rotTime.size())
								currentRotFrame = data->rotTime.size();
						}


						data->rotCurrentFrame = currentRotFrame;
					}
			}
		}

		if (animTime >= currentAnimation->endTime)
		{
			animTime = 0;
			currentAnimation = nullptr;
		}
		count++;
	}
	UpdateMatrices();
}

void Robot::LoadResources(Robot* robotmesh)
{
	//mesh has root. reason meshcollection is i-1 is due to
	if (robotmesh->meshCollection[0] == NULL)
	{
		for (int i = 1; i < skeletonParts.size(); i++)
		{
			std::string foldername;
			foldername = "Resources/" + folderName + "/" + skeletonParts[i].GetPartName() + ".x";
			meshCollection[i - 1] = CommonMesh::LoadFromXFile(Application::s_pApp, foldername.c_str());
			//^^this is needed since we need to start mesh collection from 0
		}
	}
	else
	{
		for (int i = 1; i < skeletonParts.size(); i++)
		{
			meshCollection[i - 1] = robotmesh->meshCollection[i - 1];
		}
	}

}

void Robot::ReleaseResources(void)
{
	/*for (int i = 1; i < skeletonParts.size(); i++)
	{
	skeletonParts[i].ReleaseResource();
	}*/

	for (int i = 0; i < meshCollection.size(); i++)
	{
		delete meshCollection[i];
	}
}

void Robot::DrawAll(void)
{

	for (int i = 1; i < skeletonParts.size(); i++)
	{
		Application::s_pApp->SetWorldMatrix(skeletonParts[i].GetWorldMatrix());
		meshCollection[i - 1]->Draw();
	}

}

void Robot::SetWorldPosition(float wposX, float wposY, float wposZ, float wposW, float wrotX, float wrotY, float wrotZ, float wrotW) {
	m_v4WorldPosition.x = wposX;
	m_v4WorldPosition.y = wposY;
	m_v4WorldPosition.z = wposZ;
	m_v4WorldPosition.w = wposW;

	m_v4RotationPosition.x = wrotX;
	m_v4RotationPosition.y = wrotY;
	m_v4RotationPosition.z = wrotZ;
	m_v4RotationPosition.w = wrotW;

	m_mWorldPosition = transform.CalculateLocalMatrix(m_v4WorldPosition, m_v4RotationPosition);
};

