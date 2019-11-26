#include "Robot.h"


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
		skeleton->SetSkeletonOffesetPosition(splitFloat[0] / 10, splitFloat[1] / 10, splitFloat[2] / 10, 0.f);
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
Robot::~Robot(void)
{	
}

void Robot::UpdateMatrices(void) 
{
	for (int i = 0; i < skeletonParts.size(); i++)
	{
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
	UpdateMatrices();
}

void Robot::LoadResources(Robot* robotmesh)
{
	/*for (int i = 1; i < skeletonParts.size(); i++)
	{
		skeletonParts[i].LoadResource(folderName);
	}*/
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

