#include "stdafx.h"
#include "Model.h"

void Model::Load()
{
	int nr_vertices;
	std::ifstream model_file(modelResPtr->path);
	model_file.ignore(50, ':');
	model_file >> nr_vertices;
	Vertex *verticesData = new Vertex[nr_vertices];
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
	delete[] verticesData;
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

	model_file.close();
}

void TerrainModel::Load()
{
	int n = nr_cells + 1;
	int nr_vertices = n * n;
	Vertex *verticesData = new Vertex[nr_vertices];
	int i, j, index;
	float start = (float(nr_cells) / 2 - nr_cells)*size_cell;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			index = i * n + j;
			verticesData[index].pos.x = start + i * size_cell;
			verticesData[index].pos.y = offset_y;
			verticesData[index].pos.z = start + j * size_cell;
			verticesData[index].uv.x = i;
			verticesData[index].uv.y = j;
			verticesData[index].uvBlend.x = (float)i / nr_cells;
			verticesData[index].uvBlend.y = (float)j / nr_cells;
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
	delete[] verticesData;
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

}
