#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "MeGlWindow.h"
#include "Primitive/Vertex.h"
#include "Primitive/ShapeGenerator.h"

#pragma comment(lib,"opengl32.lib") // glClearColor(), glClear ���� �Լ��� ����� �� �� ������ ���� ������ ��ũ ���� �߻�

using namespace std;

const float X_DELTA = 0.1f;
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6; // x,y,z  ,  r,g,b �������� 6���� ����.
const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
const uint MAX_TRIS = 20;

uint numTris = 0;

void sendDataToOpenGL() 
{
	ShapeData tri = ShapeGenerator::makeTriangle();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); // GL_ARRAY_BUFFER �Ǵ� GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, tri.vertexBufferSize(), tri.vertices, GL_STATIC_DRAW); // ���۸� MAX_TRIS * TRIANGLE_BYTE_SIZE ũ��� �ø���.(���빰�� NULL)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); // sizeof(float) * 6 - ���� Vertex�� �������� �̵��� �� ��ĭ �̵������� ���� ����
	glEnableVertexAttribArray(1);										   // 0 - ù �������� offset
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.indexBufferSize(), tri.indices, GL_STATIC_DRAW);
}

void sendAnotherTriToOpenGL()
{
	if(numTris == MAX_TRIS)
		return;

	const GLfloat THIS_TRI_X = -1 + numTris * X_DELTA;
	GLfloat thisTri[] = 
	{
		THIS_TRI_X, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X+X_DELTA, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	glBufferSubData(GL_ARRAY_BUFFER,  // target=Ÿ�ٹ���, offset=�ѿ�����, size=����ũ��, data=���ۿ� ������ ������ //
		numTris * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, thisTri);
	
	numTris++;
}

void MeGlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height()); // ������(�ﰢ����) ũ�⸦ ������ ũ�⿡ �� �����.
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_PER_TRI);
	
	//sendAnotherTriToOpenGL();
	//						    first     count
	//glDrawArrays(GL_TRIANGLES, 0, numTris * NUM_VERTICES_PER_TRI);
	//glDrawArrays(GL_TRIANGLES, (numTris - 1) * NUM_VERTICES_PER_TRI, NUM_VERTICES_PER_TRI); // **** front buffer, back buffer�� �̿�. �����ư��鼭 ȭ�鿡 ����Ѵ�.  **** //
																							// ȭ�鿡 ��� = Color Buffer�� �ȼ� ������ �Է�	
}

bool checkStatus(
	GLuint objectID, 
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc, // PFNGLGETSHADERIVPROC = glGetShaderiv ���� ���¹�ȯ �Լ��� �����ϴ� �Լ� ������ �ڷ���
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,	   // PFNGLGETSHADERINFOLOGPROC = glGetShaderInfoLog �� InfoLog�Լ��ּҸ� �����ϴ� ������
	GLenum statusType)
{
	GLint compileStatus;
	objectPropertyGetterFunc(objectID, statusType, &compileStatus); // ���� ���θ� 3��° ���ڿ� �ѱ��.
	//iv = integer vector
	if(compileStatus != GL_TRUE) // �������� ��� GL_FALSE ��ȯ.
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength]; // ���� �޽���(���ڿ�)�� ���� ����.

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete []buffer;
		return false;
	}	

	return true;
}

//***** ���̴� ������ ����Ȯ�� �ڵ� *****//
bool checkShaderStatus( GLuint shaderID ) 
{	
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//***** ���α׷� ��ũ ����Ȯ�� �ڵ�(������ ������ ����Ȯ�� �ڵ�� ���� ����.) *****//
bool checkProgramStatus( GLuint programID ) 
{	
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	
	if(!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}

	return std::string(
		std::istreambuf_iterator<char>(meInput), 
		std::istreambuf_iterator<char>() );
}

void installShader()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);    // Vertex Shader Object ����
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);// Fragment Shader Object ����

	const char* adapter[1];
	
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0); // 1 = adapter �迭 ũ��, adapter = �迭
												   // 0 = ???
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// �����Ͽ��� üũ //
	if( !checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID) )
		return;

	GLuint programID = glCreateProgram();		// ���α׷��� ���� ������ �� ���̴��� ���Խ���
												// ��ũ��Ű��, GPU���� �����Ų��.
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// ��ũ���� üũ //
	if( !checkProgramStatus(programID))
		return;

	glUseProgram(programID);
	
};

void MeGlWindow::initializeGL()
{
	glewInit(); // �ʱ�ȭ ���� ������ glew�Լ��� ����� �� ���� �߻�

	glEnable(GL_DEPTH_TEST); // ���� �׷����� ������ ���߿� �׷����� ������ depth������ ���ؼ�
							 // �̹����� ��ġ�� ��� depth�� ���� color�� colorBuffer�� �����Ѵ�.

	sendDataToOpenGL(); // ���� ����, ���ۿ� ��ǥ, ���� �����͸� �����Ѵ�.
	installShader();
}