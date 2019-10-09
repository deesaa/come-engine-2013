void saveFullObject(object_class* object)
{
	vertex* vertices = object->getVertices();
	WORD* indices = object->getIndices();
	DWORD numVerts = object->getNumVerts();
	DWORD numFaces = object->getNumFaces();
	DWORD* attributes = object->getAttributes();
	DWORD* adjacencyInfo = object->getAdjacencyInfo();

	int radix(8);
	int numIter(0);
	char chBuffer[20];
	char* objectName(NULL);
	objectName = itoa(object->getObjectID(), chBuffer, radix);

	std::string objectID;
	objectID.append("Objects/Object_");
	objectID.append(objectName, sizeof(objectName));

	std::ofstream objectFile(objectID.c_str());

	std::string strVertices;
	strVertices.append("Vertices in local space: \n");
	strVertices.append("(X : Y : Z : X normal : Y normal : Z normal) \n");
	for(DWORD counter(0); counter != numVerts;)
	{
		sprintf(chBuffer, "%f ; ", vertices[counter].pos.x);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", vertices[counter].pos.y);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", vertices[counter].pos.z);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", vertices[counter].nX);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", vertices[counter].nY);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%f \n", vertices[counter].nZ);
		strVertices.append(chBuffer);
		counter++;
	}
	strVertices.append("\n");
	objectFile.write(strVertices.c_str(), strVertices.size());

	std::string strIndices;
	strIndices.append("Indices: \n");
	for(DWORD counter(0); counter != numFaces;)
	{
		sprintf(chBuffer, "%d ; ", indices[counter*3]);
		strIndices.append(chBuffer);
		sprintf(chBuffer, "%d ; ", indices[counter*3+1]);
		strIndices.append(chBuffer);
		sprintf(chBuffer, "%d \n", indices[counter*3+2]);
		strIndices.append(chBuffer);
		counter++;
	}
	strIndices.append("\n");
	objectFile.write(strIndices.c_str(), strIndices.size());

	std::string strAttributes;
	strAttributes.append("Attributes: \n");
	for(DWORD counter(0); counter != numFaces;)
	{
		sprintf(chBuffer, "%d ; ", attributes[counter]);
		strAttributes.append(chBuffer);
		counter++;
	}
	strAttributes.append("\n\n");
	objectFile.write(strAttributes.c_str(), strAttributes.size());

	std::string strAdjacencyInfo;
	strAdjacencyInfo.append("Adjacency: \n");
	for(DWORD counter(0); counter != numFaces*3;)
	{
		sprintf(chBuffer, "%d ; ", adjacencyInfo[counter]);
		strAdjacencyInfo.append(chBuffer);
		counter++;
	}
	strAdjacencyInfo.append("\n\n");
	objectFile.write(strAdjacencyInfo.c_str(), strAdjacencyInfo.size());
	
	objectFile.close();
}






void saveAs(HINSTANCE bhInstace, HWND bWindow, object_class* object)
{
	int radix(8);
	char chBuffer[20];
	WCHAR objectName(NULL);
	objectName = (WCHAR)itoa(object->getObjectID(), chBuffer, radix);

	OPENFILENAME ofn;       

	char szFile[260];     
	HANDLE hf;             

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn)==TRUE) 
		hf = CreateFile(ofn.lpstrFile,  GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		0);

}