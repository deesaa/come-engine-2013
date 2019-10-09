#define _UNICODE

void saveProject(HWND mainWindow)
{
	int radix(8);
	char chBuffer[20];
	WCHAR szFile[260] = L"Project_";  
	WCHAR szFilter[260] = L".cpj\0*.*\0";  
	WCHAR szTitle[260] = L"Save Project";
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

	ofn.Flags = OFN_OVERWRITEPROMPT;

	GetSaveFileName(&ofn);
}

void saveFullObject(object_class* object)
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
	HANDLE file; 
	WCHAR* objectName;
	objectName = (WCHAR*)_itow(object->getObjectID(), (WCHAR*)chBuffer, radix);

	std::wstring objectID;
	GetCurrentDirectory(sizeof(tchBuffer_2), tchBuffer_2);
	objectID.append(tchBuffer_2);
	objectID.append(L"/Objects/Objects/Object_");
	objectID.append(objectName);
	objectID.append(L".cbj");
	file = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

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

	std::string strAttributes;
	strAttributes.append("Attributes: \n");
	for(DWORD counter(0); counter != numFaces;)
	{
		sprintf(chBuffer, "%d ; ", attributes[counter]);
		strAttributes.append(chBuffer);
		counter++;
	}
	strAttributes.append("\n\n");

	std::string strAdjacencyInfo;
	strAdjacencyInfo.append("Adjacency: \n");
	for(DWORD counter(0); counter != numFaces*3;)
	{
		sprintf(chBuffer, "%d ; ", adjacencyInfo[counter]);
		strAdjacencyInfo.append(chBuffer);
		counter++;
	}
	strAdjacencyInfo.append("\n\n");

	std::string strTriangles;
	strTriangles.append("Triangles: \n");
	for(DWORD counter(0); counter != numFaces;)
	{
		strTriangles.append("\n********************************\n");
		strTriangles.append("TriangleID: ");
		sprintf(chBuffer, "%d ; ", triangles[counter]->triangleID);
		strTriangles.append(chBuffer);
		strTriangles.append("\nVerts-Fathers: \n");
		for(DWORD counter_2(0); counter_2 != 3;)
		{
			sprintf(chBuffer, "%d ; ", triangles[counter]->fathers[counter_2]);
			strTriangles.append(chBuffer);
			counter_2 += 1;
		}
		strTriangles.append("\n");
		strTriangles.append("Verts: \n");
		for(DWORD counter_2(0); counter_2 != 3;)
		{
			sprintf(chBuffer, "%d ; ", triangles[counter]->verticesID[counter_2]);
			strTriangles.append(chBuffer);
			counter_2 += 1;
		}
		strTriangles.append("\n");
		strTriangles.append("SubsetID: \n");
		sprintf(chBuffer, "%d ; ", triangles[counter]->subsetID);
		strTriangles.append(chBuffer);

		counter++;
	}
	strTriangles.append("\n\n");

	if (file != INVALID_HANDLE_VALUE) 
	{
		WriteFile(file, strVertices.c_str(), (DWORD)strVertices.size(), &wmWritten, NULL );
		WriteFile(file, strIndices.c_str(), (DWORD)strIndices.size(), &wmWritten, NULL );
		WriteFile(file, strAttributes.c_str(), (DWORD)strAttributes.size(), &wmWritten, NULL );
		WriteFile(file, strAdjacencyInfo.c_str(), (DWORD)strAdjacencyInfo.size(), &wmWritten, NULL );
		WriteFile(file, strTriangles.c_str(), (DWORD)strTriangles.size(), &wmWritten, NULL );
	}
	CloseHandle(file);
}

void saveFullMaterial(material_class* material)
{
	int radix(10);
	char chBuffer[32];
	WCHAR tchBuffer[256];
	WCHAR tchBuffer_2[256];
	DWORD wmWritten; 
	HANDLE file; 
	WCHAR* objectName;
	objectName = (WCHAR*)_itow(material->getMaterialID(), (WCHAR*)chBuffer, radix);

	std::wstring objectID;
	GetCurrentDirectory(sizeof(tchBuffer_2), tchBuffer_2);
	objectID.append(tchBuffer_2);
	objectID.append(L"/Objects/Materials/Material_");
	objectID.append(objectName);
	objectID.append(L".cmt");
	file = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

	std::string strDiffuse;
	strDiffuse.append("Diffuse: \n");
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Diffuse.r);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Diffuse.g);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Diffuse.b);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", material->getMaterial()->Diffuse.a);
	strDiffuse.append(chBuffer);

	std::string strAmbient;
	strAmbient.append("Ambient: \n");
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Ambient.r);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Ambient.g);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Ambient.b);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", material->getMaterial()->Ambient.a);
	strAmbient.append(chBuffer);

	std::string strSpecular;
	strSpecular.append("Specular: \n");
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Specular.r);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Specular.g);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Specular.b);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", material->getMaterial()->Specular.a);
	strSpecular.append(chBuffer);

	std::string strEmissive;
	strEmissive.append("Emissive: \n");
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Emissive.r);
	strEmissive.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Emissive.g);
	strEmissive.append(chBuffer);
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Emissive.b);
	strEmissive.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", material->getMaterial()->Emissive.a);
	strEmissive.append(chBuffer);

	std::string strPower;
	strPower.append("Power: \n");
	sprintf(chBuffer, "%f ; ", material->getMaterial()->Power);
	strPower.append(chBuffer);

	if (file != INVALID_HANDLE_VALUE) 
	{
		WriteFile(file, strDiffuse.c_str(), (DWORD)strDiffuse.size(), &wmWritten, NULL );
		WriteFile(file, strAmbient.c_str(), (DWORD)strAmbient.size(), &wmWritten, NULL );
		WriteFile(file, strSpecular.c_str(), (DWORD)strSpecular.size(), &wmWritten, NULL );
		WriteFile(file, strEmissive.c_str(), (DWORD)strEmissive.size(), &wmWritten, NULL );
		WriteFile(file, strPower.c_str(), (DWORD)strPower.size(), &wmWritten, NULL );
	}
	CloseHandle(file);
}




void saveFullLight(light_class* light)
{
	int radix(8);
	char chBuffer[32];
	WCHAR tchBuffer[256];
	WCHAR tchBuffer_2[256];
	DWORD wmWritten; 
	HANDLE file; 
	WCHAR* objectName;
	objectName = (WCHAR*)_itow(light->getLightID(), (WCHAR*)chBuffer, radix);

	std::wstring objectID;
	GetCurrentDirectory(sizeof(tchBuffer_2), tchBuffer_2);
	objectID.append(tchBuffer_2);
	objectID.append(L"/Objects/Light/Light_");
	objectID.append(objectName);
	objectID.append(L".clg");
	file = CreateFile(objectID.c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

	std::string strType;
	strType.append("Type: \n");
	if(light->getLight()->Type == D3DLIGHT_DIRECTIONAL)
		strType.append("Directional ;\n");
	else
	if(light->getLight()->Type == D3DLIGHT_SPOT)
		strType.append("Spot ;\n");
	else
	if(light->getLight()->Type == D3DLIGHT_POINT)
		strType.append("Point ;\n");

	std::string strDiffuse;
	strDiffuse.append("Diffuse: \n");
	sprintf(chBuffer, "%f ; ", light->getLight()->Diffuse.r);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Diffuse.g);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Diffuse.b);
	strDiffuse.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", light->getLight()->Diffuse.a);
	strDiffuse.append(chBuffer);

	std::string strAmbient;
	strAmbient.append("Ambient: \n");
	sprintf(chBuffer, "%f ; ", light->getLight()->Ambient.r);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Ambient.g);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Ambient.b);
	strAmbient.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", light->getLight()->Ambient.a);
	strAmbient.append(chBuffer);

	std::string strSpecular;
	strSpecular.append("Specular: \n");
	sprintf(chBuffer, "%f ; ", light->getLight()->Specular.r);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Specular.g);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Specular.b);
	strSpecular.append(chBuffer);
	sprintf(chBuffer, "%f ; \n", light->getLight()->Specular.a);
	strSpecular.append(chBuffer);

	std::string strPhi;
	strPhi.append("Phi: \n");
	sprintf(chBuffer, "%f ; \n", light->getLight()->Phi);
	strPhi.append(chBuffer);

	std::string strTheta;
	strTheta.append("Theta: \n");
	sprintf(chBuffer, "%f ; \n", light->getLight()->Theta);
	strTheta.append(chBuffer);

	std::string strRange;
	strRange.append("Range: \n");
	sprintf(chBuffer, "%f ; \n", light->getLight()->Range);
	strRange.append(chBuffer);

	std::string strFalloff;
	strFalloff.append("Falloff: \n");
	sprintf(chBuffer, "%f ; \n", light->getLight()->Falloff);
	strFalloff.append(chBuffer);

	std::string strAttenuation;
	strAttenuation.append("Attenuation: \n");
	sprintf(chBuffer, "%f ; ", light->getLight()->Attenuation0);
	strAttenuation.append(chBuffer);
	sprintf(chBuffer, "%f ; ", light->getLight()->Attenuation1);
	strAttenuation.append(chBuffer);
	sprintf(chBuffer, "%f ;  \n", light->getLight()->Attenuation2);
	strAttenuation.append(chBuffer);

	if (file != INVALID_HANDLE_VALUE) 
	{
		WriteFile(file, strType.c_str(), (DWORD)strType.size(), &wmWritten, NULL );
		WriteFile(file, strDiffuse.c_str(), (DWORD)strDiffuse.size(), &wmWritten, NULL );
		WriteFile(file, strAmbient.c_str(), (DWORD)strAmbient.size(), &wmWritten, NULL );
		WriteFile(file, strSpecular.c_str(), (DWORD)strSpecular.size(), &wmWritten, NULL );
		WriteFile(file, strPhi.c_str(), (DWORD)strPhi.size(), &wmWritten, NULL );
		WriteFile(file, strTheta.c_str(), (DWORD)strTheta.size(), &wmWritten, NULL );
		WriteFile(file, strRange.c_str(), (DWORD)strRange.size(), &wmWritten, NULL );
		WriteFile(file, strFalloff.c_str(), (DWORD)strFalloff.size(), &wmWritten, NULL );
		WriteFile(file, strAttenuation.c_str(), (DWORD)strAttenuation.size(), &wmWritten, NULL );
	}
	CloseHandle(file);
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