
bool openProject(object_manager* manager, object_creator* OC)
{
	temp_manager tempManager;
	int radix(8);
	std::wstring file;
	WCHAR fileDir[260];
	WCHAR szFile[260] = L"Project";  
	WCHAR szFilter[260] = L".cpj\0*.*\0";  
	WCHAR szTitle[260] = L"Open Project";
	HANDLE hFile; 

	char chBuffer[1024];
	DWORD numOfBytes;
	ZeroMemory(chBuffer, sizeof(chBuffer));
	std::string wstrBuffer;
	std::string wstrBuffer2;
	DWORD distToObjs, distToLight, distToCams;
	DWORD found;

	OPENFILENAME ofn; 

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.nFilterIndex = 1;
	ofn.nMaxCustFilter = sizeof(szFilter);
	ofn.lpstrFilter = (LPWSTR)szFilter;
	ofn.lpstrTitle = (LPWSTR)szTitle;
	ofn.nMaxFileTitle = sizeof(szTitle);
	ofn.lpstrInitialDir = (LPWSTR)fileDir;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if(GetOpenFileName(&ofn))
	{
		OFSTRUCT fileInfo;
		hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		//std::ifstream fileStream(ofn.lpstrFile);
		ReadFile(hFile, chBuffer, sizeof(chBuffer), &numOfBytes, NULL);

		wstrBuffer.append(chBuffer);

		if(openProjectInfo(&tempManager, &wstrBuffer))
		{
			return false;
		}

		if(openObjects(&tempManager, ofn.lpstrFile) == FALSE)
		{
			MessageBox(NULL, L"Can't load Objects", NULL, MB_OK);
			return false;
		}

		if(openLights(&tempManager, ofn.lpstrFile) == FALSE)
		{
			MessageBox(NULL, L"Can't load Light", NULL, MB_OK);
			return false;
		}

		OC->unpickAll();
		OC->pickObject(manager->loadManager(&tempManager));
	}
	else
		return false;

	CloseHandle(hFile);
	return true;
}

bool openProjectInfo(temp_manager* manager, std::string* file)
{
	std::string wstrBuffer2;
	char chBuffer[1024];
	DWORD numObjects, numLight, numCams;
	DWORD dist(0);
	DWORD distToObjs(0), distToLight(0), distToCams(0);
	DWORD found(0);

	std::string str("Num Objects:");
	if((numObjects = getDwValue(&str, file, 0)) != UNKNOWN_VALUE)
		manager->numObject = numObjects;
	else
		return false;

	str = "Num Light:";
	if((numLight = getDwValue(&str, file, 0)) != UNKNOWN_VALUE)
		manager->numLight = numLight;
	else
		return false;

	str = "Num Cams:";
	if((numCams = getDwValue(&str, file, 0)) != UNKNOWN_VALUE)
		manager->numCams = numCams;
	else
		return false;

	manager->numGlobal = numObjects+numLight+numCams;
}

bool openLights(temp_manager* manager, std::wstring fileName)
{
	std::wstring projectPath;
	std::wstring pathToFile;
	std::wstring pathToLight;
	std::string  file;
	DWORD fileSize(0);
	DWORD lightNumber(0);
	WIN32_FIND_DATAW findData;
	OFSTRUCT fileInfo;
	HANDLE hFileFind, hFile;
	DWORD numOfBytes(0);
	DWORD nameSize = fileName.size();
	DWORD bDistTo(0), distTo(0);
	while(TRUE)
	{
		bDistTo = fileName.find(L"\\", distTo);
		if(bDistTo != std::wstring::npos)
			distTo = bDistTo+1;
		else
			break;
	}
	pathToFile = projectPath = pathToLight = (fileName.erase(distTo, nameSize-distTo));
	pathToLight.append(L"Light\\");
	projectPath.append(L"Light\\*.clg");

	hFileFind = FindFirstFile(projectPath.c_str(), &findData);
	if (hFileFind == INVALID_HANDLE_VALUE)
	{	return FALSE;}

	pathToFile.append(L"Light\\");
	pathToFile.append(findData.cFileName);

	hFile = CreateFile(pathToFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	fileSize = GetFileSize(hFile, NULL);
	char* pchBuffer = new char[fileSize];
	ReadFile(hFile, (void*)pchBuffer, fileSize, &numOfBytes, NULL);
	file.clear();
	file.append(pchBuffer);
	manager->addLight(&file);

	lightNumber += 1;

	while(TRUE)
	{
		if(FindNextFile(hFileFind, &findData) == NULL)
			break;

		pathToFile = pathToLight;
		pathToFile.append(findData.cFileName);

		//Похоже он как-то неправильно переключается с файла на файл
		hFile = CreateFile(pathToFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		fileSize = GetFileSize(hFile, NULL);
		delete [] pchBuffer;
		char* pchBuffer = new char[fileSize];
		ReadFile(hFile, (void*)pchBuffer, fileSize, &numOfBytes, NULL);
		file.clear();
		file.append(pchBuffer);
		manager->addLight(&file);

		lightNumber += 1;
	}


	delete [] pchBuffer;
	FindClose(hFileFind);
	CloseHandle(hFile);
	return TRUE;
}
bool openLight(tempLight_class* light, std::string* file)
{
	std::string str;
	DWORD distToStruct(0);
	std::vector<float> Diffuse;
	std::vector<float> Ambient;
	std::vector<float> Specular;
	std::vector<float> Attenuation;
	DWORD lightType;
	float Phi, Theta, Range, Falloff;

	str.clear();
	str.append("Type:");
	lightType = getDwValue(&str, file, 0);
	if (lightType == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Diffuse:");
	Diffuse.clear();
	Diffuse = getArrayFlValue(&str, file, 0);
	if (Diffuse[0] == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Ambient:");
	Ambient.clear();
	Ambient = getArrayFlValue(&str, file, 0);
	if (Ambient[0] == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Specular:");
	Specular.clear();
	Specular = getArrayFlValue(&str, file, 0);
	if (Specular[0] == UNKNOWN_VALUE)
		{	return true;}

	str.clear();
	str.append("Phi:");
	Phi = getFlValue(&str, file, 0);
	if (Phi == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Theta:");
	Theta = getFlValue(&str, file, 0);
	if (Theta == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Range:");
	Range = getFlValue(&str, file, 0);
	if (Range == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Falloff:");
	Falloff = getFlValue(&str, file, 0);
	if (Falloff == UNKNOWN_VALUE)
	{	return true;}

	str.clear();
	str.append("Attenuation:");
	Attenuation.clear();
	Attenuation = getArrayFlValue(&str, file, 0);
	if (Attenuation[0] == UNKNOWN_VALUE)
	{	return true;}

	light->loadLight(lightType, Diffuse, Ambient, Specular, Phi, Theta, Range, Falloff, Attenuation);

	return TRUE;
}
bool openObjects(temp_manager* manager, std::wstring fileName)
{
	std::wstring projectPath;
	std::wstring pathToFile;
	std::wstring pathToObjects;
	std::string  file;
	DWORD fileSize(0);
	DWORD objectNumber(0);
	WIN32_FIND_DATAW findData;
	OFSTRUCT fileInfo;
	HANDLE hFileFind, hFile;
	DWORD numOfBytes(0);
	DWORD nameSize = fileName.size();
	DWORD bDistTo(0), distTo(0);
	while(TRUE)
	{
		bDistTo = fileName.find(L"\\", distTo);
		if(bDistTo != std::wstring::npos)
			distTo = bDistTo+1;
		else
			break;
	}
	pathToFile = projectPath = pathToObjects = (fileName.erase(distTo, nameSize-distTo));
	pathToObjects.append(L"Objects\\");
	projectPath.append(L"Objects\\*.cbj");
	
	hFileFind = FindFirstFile(projectPath.c_str(), &findData);
	if (hFileFind == INVALID_HANDLE_VALUE)
	{	return FALSE;}

	pathToFile.append(L"Objects\\");
	pathToFile.append(findData.cFileName);

	hFile = CreateFile(pathToFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	fileSize = GetFileSize(hFile, NULL);
	char* pchBuffer = new char[fileSize];
	ReadFile(hFile, (void*)pchBuffer, fileSize, &numOfBytes, NULL);
	file.clear();
	file.append(pchBuffer);
	manager->addObject(&file);
	objectNumber += 1;

	while(TRUE)
	{
		if(FindNextFile(hFileFind, &findData) == NULL)
			break;

		pathToFile = pathToObjects;
		pathToFile.append(findData.cFileName);

		//Похоже он как-то неправильно переключается с файла на файл
		hFile = CreateFile(pathToFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ZeroMemory(pchBuffer, sizeof(pchBuffer));
		ReadFile(hFile, pchBuffer, sizeof(pchBuffer), &numOfBytes, NULL);
		file.clear();
		file.append(pchBuffer);
		manager->addObject(&file);

		objectNumber += 1;
	}

	delete [] pchBuffer;
	FindClose(hFileFind);
	CloseHandle(hFile);
	return TRUE;
}
bool openObjectVertices(tempObject_class* object, std::string* file)
{
	std::string str;
	std::string wstrBuffer2;
	std::vector<float> values;
	std::vector<DWORD> linkedVerts;
	char chBuffer[1024];
	DWORD vertexNumber(0);

	while(TRUE)
	{
		str.clear();
		str.append("v");
		sprintf(chBuffer, "%d", vertexNumber);
		str.append(chBuffer);
		str.append(":");

		values = getArrayFlValue(&str, file, 0);
		if (values[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("LinkedVerts");
		sprintf(chBuffer, "%d", vertexNumber);
		str.append(chBuffer);
		str.append(":");

		linkedVerts = getArrayDwValue(&str, file, 0);
		if (values[0] == UNKNOWN_VALUE)
		{	return true;}

		object->loadVertex(values, (DWORD)values.at(6), linkedVerts);
		vertexNumber += 1;
	}
}
bool openObjectIndices(tempObject_class* object, std::string* file)
{
	std::string str;									
	std::vector<DWORD> values;

	str.clear();
	str.append("Indices:");
	values.clear();
	values = getArrayDwValue(&str, file, 0);
	if (values[0] == UNKNOWN_VALUE)
	{	return true;}
	object->loadIndices(values);
}
bool openObjectAttributes(tempObject_class* object, std::string* file)
{
	std::string str;									
	std::vector<DWORD> values;

	str.clear();
	str.append("Attributes:");
	values.clear();
	values = getArrayDwValue(&str, file, 0);
	if (values[0] == UNKNOWN_VALUE)
	{	return true;}
	object->loadAttributes(values);
}
bool openObjectAdjacency(tempObject_class* object, std::string* file)
{
	std::string str;									
	std::vector<DWORD> values;

	str.clear();
	str.append("Adjacency:");
	values.clear();
	values = getArrayDwValue(&str, file, 0);
	if (values[0] == UNKNOWN_VALUE)
	{	return true;}
	object->loadAdjacency(values);
}

bool openObjectTriangles(tempObject_class* object, std::string* file)
{
	std::string str;
	CHAR chBuffer[128];
	DWORD triangleNumber(0);
	DWORD triangleID, subsetID;
	DWORD distToStruct(0);
	std::vector<DWORD> vertsFathers;
	std::vector<DWORD> verts;

	while(TRUE)
	{
		str.clear();
		str.append("TriangleID:");
		sprintf(chBuffer, "%d", triangleNumber);
		str.append(chBuffer);
		distToStruct = file->find(str.c_str());
		triangleID = getDwValue(&str, file, distToStruct);
		if (triangleID == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Verts-Fathers:");
		vertsFathers.clear();
		vertsFathers = getArrayDwValue(&str, file, distToStruct);
		if (vertsFathers[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Verts:");
		verts.clear();
		verts = getArrayDwValue(&str, file, distToStruct);
		if (verts[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("SubsetID:");
		subsetID = getDwValue(&str, file, distToStruct);
		if (subsetID == UNKNOWN_VALUE)
		{	return true;}

		object->loadTriangle(triangleID, verts, vertsFathers, subsetID);
		triangleNumber += 1;
	}
	return TRUE;
}

bool openObjectMaterials(tempObject_class* object, std::string* file)
{
	std::string str;
	std::string wstrBuffer2;
	DWORD distToStruct(0);
	std::vector<float> Diffuse;
	std::vector<float> Ambient;
	std::vector<float> Specular;
	std::vector<float> Emissive;
	float Power;
	char chBuffer[1024];
	DWORD materialID(0);
	DWORD materialNumber(0);

	while(TRUE)
	{
		str.clear();
		str.append("Material:");
		sprintf(chBuffer, "%d", materialNumber);
		str.append(chBuffer);
		distToStruct = file->find(str.c_str());
		materialID = getDwValue(&str, file, distToStruct);
		if (materialID == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Diffuse:");
		Diffuse.clear();
		Diffuse = getArrayFlValue(&str, file, distToStruct);
		if (Diffuse[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Ambient:");
		Ambient.clear();
		Ambient = getArrayFlValue(&str, file, distToStruct);
		if (Ambient[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Specular:");
		Specular.clear();
		Specular = getArrayFlValue(&str, file, distToStruct);
		if (Specular[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Emissive:");
		Emissive.clear();
		Emissive = getArrayFlValue(&str, file, distToStruct);
		if (Emissive[0] == UNKNOWN_VALUE)
		{	return true;}

		str.clear();
		str.append("Power:");
		Power = getFlValue(&str, file, distToStruct);
		if (Power == UNKNOWN_VALUE)
		{	return true;}

		object->loadMaterial(Ambient, Diffuse, Emissive, Specular, Power);
		materialNumber += 1;
	}
	return TRUE;
}
DWORD getDwValue(std::string* name, std::string* file, DWORD pos)
{
	std::string wstrBuffer2;
	DWORD nameSize(0), distTo(0), valueSize(0);
	DWORD dwValue;
	DWORD found(0);

	nameSize  = name->size();

	found = file->find(*name, pos);
	if (found != std::string::npos)
	{
		distTo = found + nameSize;
	}
	else
		return UNKNOWN_VALUE;

	found = file->find(";", distTo);
	if (found != std::string::npos)
	{
		valueSize = found - distTo;
	}
	else
		return UNKNOWN_VALUE;

	std::string strValue = file->substr(distTo, valueSize);

	return dwValue = atoi(strValue.c_str());
}
float getFlValue(std::string* name, std::string* file, DWORD pos)
{
	std::string wstrBuffer2;
	float nameSize(0), distTo(0), valueSize(0);
	float flValue;
	float found(0);

	nameSize  = name->size();

	found = file->find(*name, pos);
	if (found != std::string::npos)
	{
		distTo = found + nameSize;
	}
	else
		return UNKNOWN_VALUE;

	found = file->find(";", distTo);
	if (found != std::string::npos)
	{
		valueSize = found - distTo;
	}
	else
		return UNKNOWN_VALUE;

	std::string strValue = file->substr(distTo, valueSize);

	return flValue = (float)atof(strValue.c_str());
}
//Вот только меня мучает один вопрос: как можно слушать буквы?
std::vector<float> getArrayFlValue(std::string* name, std::string* file, DWORD pos)
{
	std::string wstrBuffer2;
	std::vector<std::string> strValues;
	std::vector<float>		 values;
	char chBuffer[1024];
	DWORD nameSize(0), distToValue(0), valueSize(0), distToEnd(0);
	DWORD dwValue;
	DWORD numValues(0);
	DWORD found(0);

	nameSize  = name->size();
	found = file->find(*name, pos);
	if (found != std::string::npos)
	{
		distToValue = found + nameSize;
	}
	else
	{	values.push_back(UNKNOWN_VALUE);
		return values;
	}

	found = file->find(";", distToValue);
	if (found != std::string::npos)
	{
		distToEnd = found;
	}
	else
	{	values.push_back(UNKNOWN_VALUE);
		return values;
	}

	while(TRUE)
	{
		if(distToValue >= distToEnd)
			break;

		found = file->find(",", distToValue);
		if (found != std::string::npos)
		{
			ZeroMemory(chBuffer, sizeof(chBuffer));
			valueSize = found - distToValue;
			sprintf(chBuffer, "%d", found);
		}

		strValues.push_back(file->substr(distToValue, valueSize));
		distToValue = distToValue + valueSize+1;
	}

	for(DWORD counter(0); counter != strValues.size();)
	{
		values.push_back((float)atof(strValues[counter].c_str()));
		counter += 1;
	}

	return values;
}

std::vector<DWORD> getArrayDwValue(std::string* name, std::string* file, DWORD pos)
{
	std::string wstrBuffer2;
	std::vector<std::string> strValues;
	std::vector<DWORD>		 values;
	char chBuffer[1024];
	DWORD nameSize(0), distToValue(0), valueSize(0), distToEnd(0);
	DWORD dwValue;
	DWORD numValues(0);
	DWORD found(0);

	nameSize  = name->size();
	found = file->find(*name, pos);
	if (found != std::string::npos)
	{
		distToValue = found + nameSize;
	}
	else
	{	values.push_back(UNKNOWN_VALUE);
		return values;}

	found = file->find(";", distToValue);
	if (found != std::string::npos)
	{
		distToEnd = found;
	}
	else
	{	values.push_back(UNKNOWN_VALUE);
		return values;}

	while(TRUE)
	{
		if(distToValue >= distToEnd)
			break;

		found = file->find(",", distToValue);
		if (found != std::string::npos)
		{
			ZeroMemory(chBuffer, sizeof(chBuffer));
			valueSize = found - distToValue;
			sprintf(chBuffer, "%d", found);
		}

		strValues.push_back(file->substr(distToValue, valueSize));
		distToValue = distToValue + valueSize+1;
	}

	for(DWORD counter(0); counter != strValues.size();)
	{
		values.push_back((DWORD)atol(strValues[counter].c_str()));
		counter += 1;
	}

	return values;
}
void saveProject(HWND mainWindow, object_manager* manager)
{
	int radix(8);
	std::wstring file;
	std::wstring fileDir;
	std::wstring objectFolder;
	std::wstring lightFolder;
	std::wstring materialFolder;

	WCHAR szFile[260] = L"Project_";  
	WCHAR szFilter[260] = L"Folder\0*.*\0";  
	WCHAR szTitle[260] = L"Save Project";

	WCHAR chBuffer[260];
	GetCurrentDirectory(sizeof(chBuffer), (LPWSTR)chBuffer);
	fileDir.append(chBuffer);
	fileDir.append(L"/Projects/");
	//CreateDirectory(fileDir.c_str(), NULL);

	OPENFILENAME ofn; 

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.nFilterIndex = 1;
	ofn.nMaxCustFilter = sizeof(szFilter);
	ofn.lpstrFilter = (LPWSTR)szFilter;
	ofn.lpstrTitle = (LPWSTR)szTitle;
	ofn.nMaxFileTitle = sizeof(szTitle);
	ofn.lpstrInitialDir = fileDir.c_str();
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn))
	{
		SECURITY_ATTRIBUTES securAttribs;
		CreateDirectory(ofn.lpstrFile, NULL);
		file.append(ofn.lpstrFile);

		objectFolder = file;
		objectFolder.append(L"/Objects");
		CreateDirectory(objectFolder.c_str(), NULL);

	//	materialFolder = file;
		//materialFolder.append(L"/Materials");
		//CreateDirectory(materialFolder.c_str(), NULL);

		for(DWORD counter(0); counter != manager->getNumObjects();)
		{
			saveFullObject(manager->getObject(counter), file);

		//	for(DWORD counter_2(0); counter_2 != manager->getNumMaterials(counter);)
			//{
			//	saveFullMaterial(manager->getMaterialClass(counter, counter_2), file);
			//	counter_2 += 1;
			//}
			counter += 1;
		}

		lightFolder = file;
		lightFolder.append(L"/Light");
		CreateDirectory(lightFolder.c_str(), NULL);
		for(DWORD counter(0); counter != manager->getNumLight();)
		{
			saveFullLight(manager->getLight(counter), file);
			counter += 1;
		}

		saveProjectInfo(manager, file);
	}
}

void saveFullObject(object_class* object, std::wstring file)
{
	vertex* vertices = object->getVertices();
	WORD* indices = object->getIndices();
	DWORD numVerts = object->getNumVerts();
	DWORD numFaces = object->getNumFaces();
	DWORD* attributes = object->getAttributes();
	DWORD* adjacencyInfo = object->getAdjacencyInfo();
	triangle** triangles = object->getTriangles();

	int radix(8);
	char chBuffer[32];
	WCHAR tchBuffer[256];
	WCHAR tchBuffer_2[256];
	DWORD wmWritten; 
	HANDLE hFile; 
	WCHAR* objectName;
	objectName = (WCHAR*)_itow(object->getObjectID(), (WCHAR*)chBuffer, radix);

	std::wstring objectID;
	GetCurrentDirectory(sizeof(tchBuffer_2), tchBuffer_2);
	objectID = file;
	objectID.append(L"/Objects/Object_");
	objectID.append(objectName);
	objectID.append(L".cbj");
	hFile = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

	std::string strVertices;
	strVertices.append("Vertices:");
	for(DWORD counter(0); counter != numVerts;)
	{
		sprintf(chBuffer, "v", vertices[counter].pos.x);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%d:", counter);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%.4f,", vertices[counter].pos.x);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%.4f,", vertices[counter].pos.y);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%.4f,", vertices[counter].pos.z);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%.4f,", vertices[counter].nX);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%.4f,", vertices[counter].nY);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%.4f,", vertices[counter].nZ);
		strVertices.append(chBuffer);
		sprintf(chBuffer, "%d,;\n", (DWORD)object->getVertexSphere(counter)->vertType);
		strVertices.append(chBuffer);

		strVertices.append("LinkedVerts");
		sprintf(chBuffer, "%d:", counter);
		strVertices.append(chBuffer);
		if(object->getVertexSphere(counter)->numLinkedVerts == 0)
			strVertices.append("-1,");

		for(DWORD counter_2(0); counter_2 != object->getVertexSphere(counter)->numLinkedVerts;)
		{
			sprintf(chBuffer, "%d,", (DWORD)object->getVertexSphere(counter)->linkedVertices[counter_2]);
			strVertices.append(chBuffer);
			counter_2 += 1;
		}
		strVertices.append(";\n");
		counter++;
	}
	strVertices.append("VertsEnd;\n");

	std::string strIndices;
	strIndices.append("Indices:");
	for(DWORD counter(0); counter != numFaces;)
	{
		sprintf(chBuffer, "%d,", indices[counter*3]);
		strIndices.append(chBuffer);
		sprintf(chBuffer, "%d,", indices[counter*3+1]);
		strIndices.append(chBuffer);
		sprintf(chBuffer, "%d,", indices[counter*3+2]);
		strIndices.append(chBuffer);
		counter++;
	}
	//strIndices.erase(strIndices.size()-1, 1);
	strIndices.append(";\n");

	std::string strAttributes;
	strAttributes.append("Attributes:");
	for(DWORD counter(0); counter != numFaces;)
	{
		sprintf(chBuffer, "%d,", attributes[counter]);
		strAttributes.append(chBuffer);
		counter++;
	}
	//strAttributes.erase(strAttributes.size()-1, 1);
	strAttributes.append(";\n");

	std::string strAdjacencyInfo;
	strAdjacencyInfo.append("Adjacency:");
	for(DWORD counter(0); counter != numFaces*3;)
	{
		sprintf(chBuffer, "%d,", adjacencyInfo[counter]);
		strAdjacencyInfo.append(chBuffer);
		counter++;
	}
	//strAdjacencyInfo.erase(strAdjacencyInfo.size()-1, 1);
	strAdjacencyInfo.append(";\n");

	std::string strTriangles;
	strTriangles.append("Triangles:\n");
	for(DWORD counter(0); counter != numFaces;)
	{
		strTriangles.append("TriangleID:");
		sprintf(chBuffer, "%d;\n", triangles[counter]->triangleID);
		strTriangles.append(chBuffer);
		strTriangles.append("Verts-Fathers:");
		for(DWORD counter_2(0); counter_2 != 3;)
		{
			sprintf(chBuffer, "%d,", triangles[counter]->fathers[counter_2]);
			strTriangles.append(chBuffer);
			counter_2 += 1;
		}
		strTriangles.append(";\n");

		strTriangles.append("Verts:");
		for(DWORD counter_2(0); counter_2 != 3;)
		{
			sprintf(chBuffer, "%d,", triangles[counter]->verticesID[counter_2]);
			strTriangles.append(chBuffer);
			counter_2 += 1;
		}
		strTriangles.append(";\n");

		strTriangles.append("SubsetID:");
		sprintf(chBuffer, "%d;", triangles[counter]->subsetID);
		strTriangles.append(chBuffer);

		counter++;
	}
	strTriangles.append("\n\n");

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		WriteFile(hFile, strVertices.c_str(), (DWORD)strVertices.size(), &wmWritten, NULL );
		WriteFile(hFile, strIndices.c_str(), (DWORD)strIndices.size(), &wmWritten, NULL );
		WriteFile(hFile, strAttributes.c_str(), (DWORD)strAttributes.size(), &wmWritten, NULL );
		WriteFile(hFile, strAdjacencyInfo.c_str(), (DWORD)strAdjacencyInfo.size(), &wmWritten, NULL );
		WriteFile(hFile, strTriangles.c_str(), (DWORD)strTriangles.size(), &wmWritten, NULL );
	}

	std::string strMaterialID;
	std::string strDiffuse;
	std::string strAmbient;
	std::string strSpecular;
	std::string strEmissive;
	std::string strPower;

	for(DWORD counter(0); counter != object->getNumMaterials();)
	{
		strMaterialID.clear();
		strDiffuse.clear();
		strAmbient.clear();
		strSpecular.clear();
		strEmissive.clear();
		strPower.clear();

		strMaterialID.append("Material:");
		sprintf(chBuffer, "%d;\n", object->getMaterialClass(counter)->getMaterialID());
		strMaterialID.append(chBuffer);

		strDiffuse.append("Diffuse:");
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Diffuse.r);
		strDiffuse.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Diffuse.g);
		strDiffuse.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Diffuse.b);
		strDiffuse.append(chBuffer);
		sprintf(chBuffer, "%f,;\n", object->getMaterialClass(counter)->getMaterial()->Diffuse.a);
		strDiffuse.append(chBuffer);

		strAmbient.append("Ambient:");
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Ambient.r);
		strAmbient.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Ambient.g);
		strAmbient.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Ambient.b);
		strAmbient.append(chBuffer);
		sprintf(chBuffer, "%f,;\n", object->getMaterialClass(counter)->getMaterial()->Ambient.a);
		strAmbient.append(chBuffer);

		strSpecular.append("Specular:");
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Specular.r);
		strSpecular.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Specular.g);
		strSpecular.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Specular.b);
		strSpecular.append(chBuffer);
		sprintf(chBuffer, "%f,;\n", object->getMaterialClass(counter)->getMaterial()->Specular.a);
		strSpecular.append(chBuffer);

		strEmissive.append("Emissive:");
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Emissive.r);
		strEmissive.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Emissive.g);
		strEmissive.append(chBuffer);
		sprintf(chBuffer, "%f,", object->getMaterialClass(counter)->getMaterial()->Emissive.b);
		strEmissive.append(chBuffer);
		sprintf(chBuffer, "%f,;\n", object->getMaterialClass(counter)->getMaterial()->Emissive.a);
		strEmissive.append(chBuffer);

		strPower.append("Power:");
		sprintf(chBuffer, "%f;\n", object->getMaterialClass(counter)->getMaterial()->Power);
		strPower.append(chBuffer);

		WriteFile(hFile, strMaterialID.c_str(), (DWORD)strMaterialID.size(), &wmWritten, NULL );
		WriteFile(hFile, strDiffuse.c_str(), (DWORD)strDiffuse.size(), &wmWritten, NULL );
		WriteFile(hFile, strAmbient.c_str(), (DWORD)strAmbient.size(), &wmWritten, NULL );
		WriteFile(hFile, strSpecular.c_str(), (DWORD)strSpecular.size(), &wmWritten, NULL );
		WriteFile(hFile, strEmissive.c_str(), (DWORD)strEmissive.size(), &wmWritten, NULL );
		WriteFile(hFile, strPower.c_str(), (DWORD)strPower.size(), &wmWritten, NULL );

		counter += 1;
	}

	CloseHandle(hFile);
}

void saveFullMaterial(material_class* material, std::wstring file)
{
	int radix(10);
	char chBuffer[32];
	WCHAR tchBuffer[256];
	WCHAR tchBuffer_2[256];
	DWORD wmWritten; 
	HANDLE hFile; 
	WCHAR* objectName;
	objectName = (WCHAR*)_itow(material->getMaterialID(), (WCHAR*)chBuffer, radix);

	std::wstring objectID;
	GetCurrentDirectory(sizeof(tchBuffer_2), tchBuffer_2);
	objectID = file;
	objectID.append(L"/Materials/Material_");
	objectID.append(objectName);
	objectID.append(L".cmt");
	hFile = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

	std::string strMaterialID;
	strMaterialID.append("Diffuse:");
	sprintf(chBuffer, "%d;\n", material->getMaterialID());
	strMaterialID.append(chBuffer);

	std::string strDiffuse;
	strDiffuse.append("Diffuse:");
	sprintf(chBuffer, "%f,", material->getMaterial()->Diffuse.r);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Diffuse.g);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Diffuse.b);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f,;\n", material->getMaterial()->Diffuse.a);
	strDiffuse.append(chBuffer);

	std::string strAmbient;
	strAmbient.append("Ambient:");
	sprintf(chBuffer, "%f,", material->getMaterial()->Ambient.r);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Ambient.g);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Ambient.b);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f,;\n", material->getMaterial()->Ambient.a);
	strAmbient.append(chBuffer);

	std::string strSpecular;
	strSpecular.append("Specular:");
	sprintf(chBuffer, "%f,", material->getMaterial()->Specular.r);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Specular.g);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Specular.b);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f,;\n", material->getMaterial()->Specular.a);
	strSpecular.append(chBuffer);

	std::string strEmissive;
	strEmissive.append("Emissive:");
	sprintf(chBuffer, "%f,", material->getMaterial()->Emissive.r);
	strEmissive.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Emissive.g);
	strEmissive.append(chBuffer);
	sprintf(chBuffer, "%f,", material->getMaterial()->Emissive.b);
	strEmissive.append(chBuffer);
	sprintf(chBuffer, "%f,;\n", material->getMaterial()->Emissive.a);
	strEmissive.append(chBuffer);

	std::string strPower;
	strPower.append("Power:");
	sprintf(chBuffer, "%f;", material->getMaterial()->Power);
	strPower.append(chBuffer);

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		WriteFile(hFile, strMaterialID.c_str(), (DWORD)strMaterialID.size(), &wmWritten, NULL );
		WriteFile(hFile, strDiffuse.c_str(), (DWORD)strDiffuse.size(), &wmWritten, NULL );
		WriteFile(hFile, strAmbient.c_str(), (DWORD)strAmbient.size(), &wmWritten, NULL );
		WriteFile(hFile, strSpecular.c_str(), (DWORD)strSpecular.size(), &wmWritten, NULL );
		WriteFile(hFile, strEmissive.c_str(), (DWORD)strEmissive.size(), &wmWritten, NULL );
		WriteFile(hFile, strPower.c_str(), (DWORD)strPower.size(), &wmWritten, NULL );
	}
	CloseHandle(hFile);
}




void saveFullLight(light_class* light, std::wstring file)
{
	int radix(8);
	char chBuffer[32];
	WCHAR tchBuffer[256];
	WCHAR tchBuffer_2[256];
	DWORD wmWritten; 
	HANDLE hFile; 
	WCHAR* objectName;
	objectName = (WCHAR*)_itow(light->getLightID(), (WCHAR*)chBuffer, radix);

	std::wstring objectID;
	GetCurrentDirectory(sizeof(tchBuffer_2), tchBuffer_2);
	objectID = file;
	objectID.append(L"/Light/Light_");
	objectID.append(objectName);
	objectID.append(L".clg");
	hFile = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

	std::string strType;
	strType.append("Type:");
	sprintf(chBuffer, "%d;\n", (DWORD)light->getLight()->Type);
	strType.append(chBuffer);

	std::string strDiffuse;
	strDiffuse.append("Diffuse:");
	sprintf(chBuffer, "%.4f,", light->getLight()->Diffuse.r);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Diffuse.g);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Diffuse.b);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%.4f,;\n", light->getLight()->Diffuse.a);
	strDiffuse.append(chBuffer);

	std::string strAmbient;
	strAmbient.append("Ambient:");
	sprintf(chBuffer, "%.4f,", light->getLight()->Ambient.r);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Ambient.g);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Ambient.b);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%.4f,;\n", light->getLight()->Ambient.a);
	strAmbient.append(chBuffer);

	std::string strSpecular;
	strSpecular.append("Specular:");
	sprintf(chBuffer, "%.4f,", light->getLight()->Specular.r);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Specular.g);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Specular.b);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%.4f,;\n", light->getLight()->Specular.a);
	strSpecular.append(chBuffer);

	std::string strPhi;
	strPhi.append("Phi:");
	sprintf(chBuffer, "%.4f;\n", light->getLight()->Phi);
	strPhi.append(chBuffer);

	std::string strTheta;
	strTheta.append("Theta:");
	sprintf(chBuffer, "%.4f;\n", light->getLight()->Theta);
	strTheta.append(chBuffer);

	std::string strRange;
	strRange.append("Range:");
	sprintf(chBuffer, "%.4f;\n", light->getLight()->Range);
	strRange.append(chBuffer);

	std::string strFalloff;
	strFalloff.append("Falloff:");
	sprintf(chBuffer, "%.4f;\n", light->getLight()->Falloff);
	strFalloff.append(chBuffer);

	std::string strAttenuation;
	strAttenuation.append("Attenuation:");
	sprintf(chBuffer, "%.4f,", light->getLight()->Attenuation0);
	strAttenuation.append(chBuffer);
	sprintf(chBuffer, "%.4f,", light->getLight()->Attenuation1);
	strAttenuation.append(chBuffer);
	sprintf(chBuffer, "%.4f,;\n", light->getLight()->Attenuation2);
	strAttenuation.append(chBuffer);

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		WriteFile(hFile, strType.c_str(), (DWORD)strType.size(), &wmWritten, NULL );
		WriteFile(hFile, strDiffuse.c_str(), (DWORD)strDiffuse.size(), &wmWritten, NULL );
		WriteFile(hFile, strAmbient.c_str(), (DWORD)strAmbient.size(), &wmWritten, NULL );
		WriteFile(hFile, strSpecular.c_str(), (DWORD)strSpecular.size(), &wmWritten, NULL );
		WriteFile(hFile, strPhi.c_str(), (DWORD)strPhi.size(), &wmWritten, NULL );
		WriteFile(hFile, strTheta.c_str(), (DWORD)strTheta.size(), &wmWritten, NULL );
		WriteFile(hFile, strRange.c_str(), (DWORD)strRange.size(), &wmWritten, NULL );
		WriteFile(hFile, strFalloff.c_str(), (DWORD)strFalloff.size(), &wmWritten, NULL );
		WriteFile(hFile, strAttenuation.c_str(), (DWORD)strAttenuation.size(), &wmWritten, NULL );
	}
	CloseHandle(hFile);
}
void saveProjectInfo(object_manager* manager, std::wstring file)
{
	int radix(8);
	char chBuffer[32];
	WCHAR tchBuffer[256];
	WCHAR tchBuffer_2[256];
	DWORD wmWritten; 
	HANDLE hFile; 
	WCHAR* objectName;

	std::wstring objectID;
	objectID = file;
	objectID.append(L"/Project");
	objectID.append(L".cpj");
	hFile = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

	std::string strInfo;
	strInfo.append("Num Objects:");
	sprintf(chBuffer, "%d;\n", manager->getNumObjects());
	strInfo.append(chBuffer);

	strInfo.append("Num Light:");
	sprintf(chBuffer, "%d;\n", manager->getNumLight());
	strInfo.append(chBuffer);

	strInfo.append("Num Cams:");
	sprintf(chBuffer, "%d;\n", manager->getNumCams());
	strInfo.append(chBuffer);

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		WriteFile(hFile, strInfo.c_str(), (DWORD)strInfo.size(), &wmWritten, NULL );
	}

	CloseHandle(hFile);
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