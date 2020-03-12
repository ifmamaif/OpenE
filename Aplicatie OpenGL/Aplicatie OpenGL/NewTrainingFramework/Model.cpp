#include "stdafx.h"
#include "Model.h"

void Model::generateNormals()
{
	int i;
	nrIndicesNormals = 2 * nr_vertices;
	Vertex *verticesDataNormals = new Vertex[nrIndicesNormals];
	for (i = 0; i < nr_vertices; i++)
	{
		verticesDataNormals[2 * i].pos = verticesData[i].pos;
		verticesDataNormals[2 * i].norm = Vector3(0, 0, 0);
		verticesDataNormals[2 * i].color = Vector3(1, 0, 0);

		verticesDataNormals[2 * i + 1].pos = verticesData[i].pos;
		verticesDataNormals[2 * i + 1].norm = verticesData[i].norm;
		verticesDataNormals[2 * i + 1].color = Vector3(0, 1, 0);
	}
	glGenBuffers(1, &vboNormalsId);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId);
	glBufferData(GL_ARRAY_BUFFER, nrIndicesNormals * sizeof(Vertex), verticesDataNormals, GL_STATIC_DRAW);
	delete[] verticesDataNormals;
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::Load()
{
	//int nr_vertices;
	std::ifstream model_file(modelResPtr->path);
	model_file.ignore(50, ':');
	model_file >> nr_vertices;
	verticesData = new Vertex[nr_vertices];
	int i;
	for (i = 0; i < nr_vertices; i++)
	{
		model_file.ignore(50, '[');
		model_file >> verticesData[i].pos.x;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].pos.y;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].pos.z;
		model_file.ignore(50, '[');
		model_file >> verticesData[i].norm.x;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].norm.y;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].norm.z;
		model_file.ignore(50, '[');
		model_file >> verticesData[i].binorm.x;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].binorm.y;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].binorm.z;
		model_file.ignore(50, '[');
		model_file >> verticesData[i].tgt.x;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].tgt.y;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].tgt.z;
		model_file.ignore(50, '[');
		model_file >> verticesData[i].uv.x;
		model_file.ignore(10, ',');
		model_file >> verticesData[i].uv.y;
		verticesData[i].color = Vector3(1, 0, 0);
	}
	model_file.ignore(50, ':');
	model_file >> nrIndices;
	wired_nrIndices = nrIndices * 2;
	GLushort *indicesData = new GLushort[nrIndices];
	GLushort *wired_indicesData = new GLushort[wired_nrIndices];
	int nr_triangles = nrIndices / 3;
	
	for (i = 0; i < nr_triangles; i++)
	{
		model_file.ignore(50, '.');
		model_file >> indicesData[i * 3];	
		model_file.ignore(10, ',');
		model_file >> indicesData[i * 3 + 1];	
		model_file.ignore(10, ',');
		model_file >> indicesData[i * 3 + 2];
		//line 1
		wired_indicesData[i * 6] = indicesData[i * 3];
		wired_indicesData[i * 6 + 1] = indicesData[i * 3 + 1];
		//line 2
		wired_indicesData[i * 6 + 2] = indicesData[i * 3 + 1];
		wired_indicesData[i * 6 + 3] = indicesData[i * 3 + 2];
		//line 3
		wired_indicesData[i * 6 + 4] = indicesData[i * 3 + 2];
		wired_indicesData[i * 6 + 5] = indicesData[i * 3];
	}

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, nr_vertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	//delete[] verticesData;
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndices * sizeof(GLushort), indicesData, GL_STATIC_DRAW);
	delete[] indicesData;
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &wired_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wired_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, wired_nrIndices * sizeof(GLushort), wired_indicesData, GL_STATIC_DRAW);
	delete[] wired_indicesData;
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	generateNormals();
	model_file.close();
}

void Model::getAABB(float * const & extremePoints, Vertex * &AABB, GLushort *& indices)
{
	//AABB = new Vertex[8];	
	/*
	AABB[maxim::stanga_sus_fata].pos = Vector3(extremePoints[stanga], extremePoints[sus], extremePoints[fata]);
	*/

	AABB[maxim::stanga_sus_fata].pos.x = extremePoints[stanga];
	AABB[maxim::stanga_sus_fata].pos.y = extremePoints[sus];
	AABB[maxim::stanga_sus_fata].pos.z = extremePoints[fata];
	AABB[maxim::stanga_sus_fata].color = Vector3(0, 0, 1);

	AABB[maxim::stanga_sus_spate].pos.x = extremePoints[stanga];
	AABB[maxim::stanga_sus_spate].pos.y = extremePoints[sus];
	AABB[maxim::stanga_sus_spate].pos.z = extremePoints[spate];
	AABB[maxim::stanga_sus_spate].color = Vector3(0, 0, 1);

	AABB[maxim::stanga_jos_fata].pos.x = extremePoints[stanga];
	AABB[maxim::stanga_jos_fata].pos.y = extremePoints[jos];
	AABB[maxim::stanga_jos_fata].pos.z = extremePoints[fata];
	AABB[maxim::stanga_jos_fata].color = Vector3(0, 0, 1);

	AABB[maxim::stanga_jos_spate].pos.x = extremePoints[stanga];
	AABB[maxim::stanga_jos_spate].pos.y = extremePoints[jos];
	AABB[maxim::stanga_jos_spate].pos.z = extremePoints[spate];
	AABB[maxim::stanga_jos_spate].color = Vector3(0, 0, 1);

	AABB[maxim::dreapta_sus_fata].pos.x = extremePoints[dreapta];
	AABB[maxim::dreapta_sus_fata].pos.y = extremePoints[sus];
	AABB[maxim::dreapta_sus_fata].pos.z = extremePoints[fata];
	AABB[maxim::dreapta_sus_fata].color = Vector3(0, 0, 1);

	AABB[maxim::dreapta_sus_spate].pos.x = extremePoints[dreapta];
	AABB[maxim::dreapta_sus_spate].pos.y = extremePoints[sus];
	AABB[maxim::dreapta_sus_spate].pos.z = extremePoints[spate];
	AABB[maxim::dreapta_sus_spate].color = Vector3(0, 0, 1);

	AABB[maxim::dreapta_jos_fata].pos.x = extremePoints[dreapta];
	AABB[maxim::dreapta_jos_fata].pos.y = extremePoints[jos];
	AABB[maxim::dreapta_jos_fata].pos.z = extremePoints[fata];
	AABB[maxim::dreapta_jos_fata].color = Vector3(0, 0, 1);

	AABB[maxim::dreapta_jos_spate].pos.x = extremePoints[dreapta];
	AABB[maxim::dreapta_jos_spate].pos.y = extremePoints[jos];
	AABB[maxim::dreapta_jos_spate].pos.z = extremePoints[spate];
	AABB[maxim::dreapta_jos_spate].color = Vector3(0, 0, 1);
	//indices = new GLushort[24];
	indices[0] = maxim::dreapta_jos_fata;
	indices[1] = maxim::dreapta_jos_spate;
	indices[2] = maxim::dreapta_sus_fata;
	indices[3] = maxim::dreapta_sus_spate;
	indices[4] = maxim::dreapta_jos_fata;
	indices[5] = maxim::dreapta_sus_fata;
	indices[6] = maxim::dreapta_jos_spate;
	indices[7] = maxim::dreapta_sus_spate;
	indices[8] = maxim::stanga_jos_fata;
	indices[9] = maxim::stanga_jos_spate;
	indices[10] = maxim::stanga_sus_fata;
	indices[11] = maxim::stanga_sus_spate;
	indices[12] = maxim::stanga_jos_fata;
	indices[13] = maxim::stanga_sus_fata;
	indices[14] = maxim::stanga_jos_spate;
	indices[15] = maxim::stanga_sus_spate;
	indices[16] = maxim::stanga_jos_fata;
	indices[17] = maxim::dreapta_jos_fata;
	indices[18] = maxim::stanga_jos_spate;
	indices[19] = maxim::dreapta_jos_spate;
	indices[20] = maxim::stanga_sus_fata;
	indices[21] = maxim::dreapta_sus_fata;
	indices[22] = maxim::stanga_sus_spate;
	indices[23] = maxim::dreapta_sus_spate;
}

 void Model::getExtremePoints(Matrix & MVP, float * &extremePoints)
{
	//float* extremePoints = new float[6];
	Vector4 point;	
		point = Vector4(verticesData[0].pos) * MVP;
		extremePoints[sus] = point.y;
		extremePoints[jos] = point.y;
		extremePoints[stanga] = point.x;
		extremePoints[dreapta] = point.x;
		extremePoints[fata] = point.z;
		extremePoints[spate] = point.z;
	for (int i = 1; i < nr_vertices; i++)
	{
		point = Vector4(verticesData[i].pos) * MVP;
		if (extremePoints[sus] < point.y)
			extremePoints[sus] = point.y;
		if (extremePoints[jos] > point.y)
			extremePoints[jos] = point.y;
		if (extremePoints[dreapta] < point.x)
			extremePoints[dreapta] = point.x;
		if (extremePoints[stanga] > point.x)
			extremePoints[stanga] = point.x;
		if (extremePoints[spate] < point.z)
			extremePoints[spate] = point.z;
		if (extremePoints[fata] > point.z)
			extremePoints[fata] = point.z;	
	}
}


void TerrainModel::Load()
{
	int n = nr_cells + 1;
	nr_vertices = n * n;
	verticesData = new Vertex[nr_vertices];
	int i, j, index;
	float start = (float(nr_cells) / 2 - nr_cells)*size_cell;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			index = i * n + j;
			verticesData[index].pos.x = start + i * size_cell;
			verticesData[index].pos.y = 0;
			verticesData[index].pos.z = start + j * size_cell;
			verticesData[index].norm = Vector3(0, 1, 0);
			verticesData[index].uv.x = i;
			verticesData[index].uv.y = j;
			verticesData[index].uvBlend.x = (float)i / nr_cells;
			verticesData[index].uvBlend.y = (float)j / nr_cells;
			verticesData[index].color = Vector3(1, 0, 0);
		}
	nrIndices = nr_cells*nr_cells*6;
	wired_nrIndices = nrIndices * 2;
	GLushort *indicesData = new GLushort[nrIndices];
	GLushort *wired_indicesData = new GLushort[wired_nrIndices];
	index = 0;
	int wired_index=0;
	
	int p1, p2, p3, p4;

	for (i = 0; i < nr_cells; i++)
		for (j = 0; j < nr_cells; j++)
	{
			p1 = i * n + j;
			p2 = i * n + j + 1;
			p3 = (i + 1) * n + j;
			p4 = (i + 1) * n + j + 1;
			//triangle 1
		indicesData[index++] = p1;
		indicesData[index++] = p2;
		indicesData[index++] = p3;
		//triangle 2
		indicesData[index++] = p3;
		indicesData[index++] = p2;
		indicesData[index++] = p4;
		//line 1
		wired_indicesData[wired_index++] = p1;
		wired_indicesData[wired_index++] = p2;
		//line 2
		wired_indicesData[wired_index++] = p2;
		wired_indicesData[wired_index++] = p3;
		//line 3
		wired_indicesData[wired_index++] = p3;
		wired_indicesData[wired_index++] = p1;
		//line 4
		wired_indicesData[wired_index++] = p3;
		wired_indicesData[wired_index++] = p2;
		//line 5
		wired_indicesData[wired_index++] = p2;
		wired_indicesData[wired_index++] = p4;
		//line 6
		wired_indicesData[wired_index++] = p4;
		wired_indicesData[wired_index++] = p3;
	}
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, nr_vertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	//delete[] verticesData;
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndices * sizeof(GLushort), indicesData, GL_STATIC_DRAW);
	delete[] indicesData;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &wired_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wired_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, wired_nrIndices * sizeof(GLushort), wired_indicesData, GL_STATIC_DRAW);
	delete[] wired_indicesData;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	generateNormals();
}
