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

Robot::Robot(std::string filepath)
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
		skeleton->partName = textLineFromFile;

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

		skeleton->m_v4SkeletonPositionOffset.x = splitFloat[0]/10;
		skeleton->m_v4SkeletonPositionOffset.y = splitFloat[1]/10;
		skeleton->m_v4SkeletonPositionOffset.z = splitFloat[2]/10;
		skeleton->m_v4SkeletonPositionOffset.w = { 0.f };
		
		splitFloat.clear();

		skeleton->m_v4SkeletonRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

		skeletonParts.push_back(*skeleton);
		delete skeleton;
	}

	for (int i = 1; i < skeletonParts.size() ; i++) //Start at one because you don't need a mesh for the root
	{
		static CommonMesh* s_sMesh;
		meshCollection.push_back(s_sMesh);
	}

}

Robot::~Robot(void)
{	
}

void Robot::UpdateMatrices(void) 
{
	XMFLOAT4 test(0.0f, 20.0f, 0.0f, 0.f);
	XMMATRIX WP = XMMatrixTranslationFromVector(XMLoadFloat4(& test));
	for (int i = 0; i < skeletonParts.size(); i++)
	{
		if (skeletonPartsParent[i] == 0)
		{
			skeletonParts[0].m_mSkeletonLocalMatrix = transform.CalculateLocalMatrix(skeletonParts[0].m_v4SkeletonPositionOffset,
				skeletonParts[0].m_v4SkeletonRotation);

			skeletonParts[0].m_mSkeletonWorldMatrix = transform.CalculateWorldMatrix(skeletonParts[i].m_mSkeletonLocalMatrix, WP);//skeletonParts[0].m_mSkeletonLocalMatrix;//transform.CalculateWorldMatrix(skeletonParts[i].m_mSkeletonLocalMatrix);
		}
		else
		{
			skeletonParts[i].m_mSkeletonLocalMatrix = transform.CalculateLocalMatrix(skeletonParts[i].m_v4SkeletonPositionOffset,
				skeletonParts[i].m_v4SkeletonRotation);
			if (skeletonPartsParent[i] == 1)
			{
				//Since the parent is the world matrix then we don't need the third param
				skeletonParts[i].m_mSkeletonWorldMatrix = transform.CalculateWorldMatrix(skeletonParts[i].m_mSkeletonLocalMatrix, skeletonParts[skeletonPartsParent[i]-1].m_mSkeletonLocalMatrix, WP);
			}
			else
			{
				skeletonParts[i].m_mSkeletonWorldMatrix = transform.CalculateWorldMatrix(skeletonParts[i].m_mSkeletonLocalMatrix, skeletonParts[skeletonPartsParent[i]-1].m_mSkeletonWorldMatrix);
			}
		}
	}
}

void Robot::Update() 
{
	UpdateMatrices();
}

void Robot::LoadResources(void)
{
	/*for (int i = 1; i < skeletonParts.size(); i++)
	{
		skeletonParts[i].LoadResource(folderName);
	}*/
	
	for (int i = 1; i < skeletonParts.size(); i++)
	{
		std::string foldername;
		foldername = "Resources/" + folderName + "/" + skeletonParts[i].partName + ".x";
		meshCollection[i - 1] = CommonMesh::LoadFromXFile(Application::s_pApp, foldername.c_str());
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
		Application::s_pApp->SetWorldMatrix(skeletonParts[i].m_mSkeletonWorldMatrix);
		meshCollection[i - 1]->Draw();
	}
	
}

