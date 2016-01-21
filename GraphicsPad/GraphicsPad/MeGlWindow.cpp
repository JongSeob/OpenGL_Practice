#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include "MeGlWindow.h"

using namespace std;

#pragma comment(lib,"opengl32.lib") // glClearColor(), glClear ���� �Լ��� ����� �� �� ������ ���� ������ ��ũ ���� �߻�

void sendDataToOpenGL() 
{
	// vertex = ������
	GLfloat verts[] =    // ȭ���� �߽���ǥ = (0, 0) ������ ��ǥ = (1, 0)  ���� ��ǥ(0, 1)
	{
			+0.0f, +1.0f,   //                 index = 2
			+1.0f, +0.0f, +0.0f,
			-1.0f, -1.0f,   //                 index = 3
			+0.0f, +1.0f, +0.0f,
			+1.0f, -1.0f,   //                 index = 4
			+0.0f, +0.0f, +1.0f,

// 		+0.0f, +0.0f,   // ù ��° �ﰢ��  index = 0
// 		+1.0f, +0.0f, +0.0f,  // �������� RGB ������
// 		+1.0f, +1.0f,   //                 index = 1
// 		+1.0f, +0.0f, +0.0f,
// 		-1.0f, +1.0f,   //                 index = 2
// 		+1.0f, +0.0f, +0.0f,
// 		-1.0f, -1.0f,   //                 index = 3
// 		+1.0f, +0.0f, +0.0f,
// 		+1.0f, -1.0f,   //                 index = 4
// 		+1.0f, +0.0f, +0.0f,
	};

	/*
	*	������ ��ǥ�� �̿��� �ﰢ���� �׸��� 2���� ���
	*
	*  1. ARRAY ���ۿ� ��� �������� ������ �� ���������� �о� �������� ���Ѵ�. (�ߺ��Ǵ� �������� ��� ��������Ѵ�.)
	*  2. 1.���� ����� ARRAY���ۿ� �ε����� �����ؼ� �������� ���Ѵ�.			 (�ε����� �����ϹǷ� �ߺ��Ǵ� �������� �ѹ��� �����ص� �ȴ�.)
	*/


	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); // GL_ARRAY_BUFFER �Ǵ� GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), 
		verts, GL_STATIC_DRAW);

	

	/*
	 *	glEnableVertexAttribArray(0); ������ ���� 0 ��,
	 *  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); ������ ù ��° ���� 0��
	 *  �� �� index�� �ǹ��Ѵ�. �̰��� Vertex �� ��ǥ�� ������ �������� �����.
	 *
	 *  �׸��� glVertexAttribPointer�� 2,3 ��° ���ڴ� �ش� ������ ���ڰ� 2���� float���� �̷���� �ִ� ���� ���Ѵ�.
	 *
	 *  ��������� ������ MeShaderCode.cpp ���� "in layout(location=0) vec2 position;" �� ���� ��
	 *  location=0�� ���� 0, vec2�� ���� 2�� ��Ī�ȴ�.
	 */

	// verts[] �迭���� Vertex ��ǥ ��ġ
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);  // 5��° ���� = strider = �����Ͱ� ���� ���ڸ� ����ų �� �ּҸ� �� �������Ѿ� �ϴ����� ���� ����
	
	// verts[] �迭���� Vertex�� ���� ��ġ
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2)); // 6��° ���� = ó�� ����Ű�� ��ġ

	GLushort indices[] = {0,1,2};
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);
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

	sendDataToOpenGL(); // ���� ����, ���ۿ� ��ǥ, ���� �����͸� �����Ѵ�.
	installShader();
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height()); // ������(�ﰢ����) ũ�⸦ ������ ũ�⿡ �� �����.
	//glDrawArrays(GL_TRIANGLES, 0, 6);	 // 3��° ���� = ������ ����

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	
	

	// ȭ���� �Ѱ��� �������� ĥ�Ѵ�.
	//glClearColor(0, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
}