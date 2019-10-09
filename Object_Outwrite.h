void saveFullObject(object_class* object)
{
	int radix(8);
	char chBuffer[20];
	char* objectName(NULL);
	objectName = itoa(object->getObjectID(), chBuffer, radix);

	std::string objectID;
	objectID.append("Object_");
	objectID.append(objectName, sizeof(objectName));
	std::ofstream objectFile(objectID.c_str());

	std::string vertices;
	vertices.append("Vertices in local space: \n");
	vertices.append("(X : Y : Z : X normal : Y normal : Z normal) \n");
	object->getMehs()->LockVertexBuffer(D3DLOCK_READONLY, (void**)object->getVertices());
	for(DWORD counter(0); counter != object->getNumVerts();)
	{
		sprintf(chBuffer, "%f ; ", object->getVertices()[counter].pos.x);
		vertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", object->getVertices()[counter].pos.y);
		vertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", object->getVertices()[counter].pos.z);
		vertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", object->getVertices()[counter].nX);
		vertices.append(chBuffer);
		sprintf(chBuffer, "%f ; ", object->getVertices()[counter].nY);
		vertices.append(chBuffer);
		sprintf(chBuffer, "%f \n", object->getVertices()[counter].nZ);
		vertices.append(chBuffer);
		counter++;
	}
	vertices.append("\n");
	object->getMehs()->UnlockVertexBuffer();
	objectFile.write(vertices.c_str(), vertices.size());

	std::string indices;
	indices.append("Indices: \n");
	object->getMehs()->LockIndexBuffer(D3DLOCK_READONLY, (void**)object->getIndices());
	indices.append("0 ; "); indices.append("1 ; ");	indices.append("2 \n");	 			 //!!!ÀÑÒÀÐÎÍÛ-ÁÛÄËÀÊÀÄ!!!
	for(DWORD counter(1); counter != object->getNumFaces();)
	{
		sprintf(chBuffer, "%d ; ", (int)object->getIndices()[counter*3]);
		indices.append(chBuffer);
		sprintf(chBuffer, "%d ; ", (int)object->getIndices()[counter*3+1]);
		indices.append(chBuffer);
		sprintf(chBuffer, "%d \n", (int)object->getIndices()[counter*3+2]);
		indices.append(chBuffer);
		counter++;
	}
	indices.append("\n");
	object->getMehs()->UnlockIndexBuffer();
	objectFile.write(indices.c_str(), indices.size());

	objectFile.close();
}