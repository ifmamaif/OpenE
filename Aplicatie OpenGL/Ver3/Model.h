#pragma once
#include <iostream>
#include <fstream>

#include "../Utilities/utilities.h"
#include "Vertex.h"

using namespace std;

enum class draw_type : int {
	DRAW_FULL = 1,
	DRAW_WIRED = -1,
	DRAW_NORMALS = 10
};

enum maxim
{
	sus = 0,
	jos,
	stanga,
	dreapta,
	fata,
	spate,
	stanga_sus_fata = 0,
	stanga_sus_spate,
	dreapta_sus_fata,
	dreapta_sus_spate,
	stanga_jos_fata,
	stanga_jos_spate,
	dreapta_jos_fata,
	dreapta_jos_spate,
};

class ModelResource
{
private:
	friend class Model;
	char* path;	
public:
	ModelResource(char * const &_path):path(_path){}
	ModelResource() { cout << "Constructor implicit apelat - ModelResource: Undefined behaviour.\n"; }
	~ModelResource() { delete[] path; }
};

class Model
{
protected:
	ModelResource* modelResPtr; 
	GLuint iboId; //Index Buffer Object Id
	GLuint wired_iboId;
	GLuint vboId; //Vertex Buffer Object Id
	GLuint nrIndices, wired_nrIndices;

	GLuint vboNormalsId, nrIndicesNormals;

	Vertex* verticesData;
	int nr_vertices;
	void generateNormals();
public:
	Model(ModelResource * const &_modelResPtr) :modelResPtr(_modelResPtr), verticesData(NULL) {}
	Model() { cout << "Constructor implicit apelat - Model: Undefined behaviour.\n"; }
	virtual void Load();
	void Bind(draw_type const &type) {
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		if (type == draw_type::DRAW_FULL)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		else if (type == draw_type::DRAW_WIRED)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wired_iboId);
		else if (type == draw_type::DRAW_NORMALS)
			glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId);
		}
	void Draw(draw_type const &type)
	{
		if (type == draw_type::DRAW_FULL)
			glDrawElements(GL_TRIANGLES, nrIndices, GL_UNSIGNED_SHORT, 0);
		else if (type == draw_type::DRAW_WIRED)
			glDrawElements(GL_LINES, wired_nrIndices, GL_UNSIGNED_SHORT, 0);
		else if (type == draw_type::DRAW_NORMALS)
			glDrawArrays(GL_LINES, 0, nrIndicesNormals);	
	}

	static void getAABB(float * const &extremePoints, Vertex * &AABB, GLushort * &indices);
	void getExtremePoints(Matrix &MVP, float * &extremePoints);
	~Model() {
		if (verticesData != NULL)
		{
			glDeleteBuffers(1, &iboId);
			glDeleteBuffers(1, &wired_iboId);
			glDeleteBuffers(1, &vboId);
			glDeleteBuffers(1, &vboNormalsId);
			delete[] verticesData;
		}
	}
};

class TerrainModel : public Model 
{
private:
	GLuint nr_cells;
	GLfloat size_cell, offset_y;
public:
	TerrainModel(GLuint const &_nr_cells, GLfloat const &_size_cell, GLfloat const &_offset_y) :
		Model(NULL), nr_cells(_nr_cells), size_cell(_size_cell), offset_y(_offset_y) {};
	void Load();
	GLfloat GetSizeCell() { return size_cell; }
	GLuint GetNrCells() { return nr_cells; }
};