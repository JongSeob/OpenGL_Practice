#ifndef ShapeData_h__
#define ShapeData_h__

#include <GL/glew.h>
#include "Vertex.h"

struct ShapeData
{
	ShapeData() :
		vertices(0), numVertices(0), 
		indices(0), numIndices(0) {}
	void cleanup()
	{
		delete []vertices;
		delete []indices;
		numVertices = numIndices = 0;
	}

	Vertex* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numIndices;
	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLushort);
	}
};

#endif // ShapeData_h__
