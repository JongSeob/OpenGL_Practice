#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "MeGlWindow.h"
#include "Primitive/Vertex.h"
#include "Primitive/ShapeGenerator.h"

#pragma comment(lib,"opengl32.lib") // glClearColor(), glClear 등의 함수를 사용할 때 이 구문을 넣지 않으면 링크 에러 발생

using namespace std;

const float X_DELTA = 0.1f;
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6; // x,y,z  ,  r,g,b 꼭짓점당 6개의 정보.
const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
const uint MAX_TRIS = 20;

uint numTris = 0;

void sendDataToOpenGL() 
{
	ShapeData tri = ShapeGenerator::makeTriangle();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); // GL_ARRAY_BUFFER 또는 GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, tri.vertexBufferSize(), tri.vertices, GL_STATIC_DRAW); // 버퍼를 MAX_TRIS * TRIANGLE_BYTE_SIZE 크기로 늘린다.(내용물은 NULL)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); // sizeof(float) * 6 - 다음 Vertex의 성분으로 이동할 때 몇칸 이동할지에 대한 정보
	glEnableVertexAttribArray(1);										   // 0 - 첫 데이터의 offset
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

	glBufferSubData(GL_ARRAY_BUFFER,  // target=타겟버퍼, offset=총오프셋, size=개당크기, data=버퍼에 저장할 데이터 //
		numTris * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, thisTri);
	
	numTris++;
}

void MeGlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height()); // 도형의(삼각형의) 크기를 윈도우 크기에 딱 맞춘다.
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_PER_TRI);
	
	//sendAnotherTriToOpenGL();
	//						    first     count
	//glDrawArrays(GL_TRIANGLES, 0, numTris * NUM_VERTICES_PER_TRI);
	//glDrawArrays(GL_TRIANGLES, (numTris - 1) * NUM_VERTICES_PER_TRI, NUM_VERTICES_PER_TRI); // **** front buffer, back buffer를 이용. 번갈아가면서 화면에 출력한다.  **** //
																							// 화면에 출력 = Color Buffer에 픽셀 데이터 입력	
}

bool checkStatus(
	GLuint objectID, 
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc, // PFNGLGETSHADERIVPROC = glGetShaderiv 등의 상태반환 함수를 저장하는 함수 포인터 자료형
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,	   // PFNGLGETSHADERINFOLOGPROC = glGetShaderInfoLog 등 InfoLog함수주소를 저장하는 포인터
	GLenum statusType)
{
	GLint compileStatus;
	objectPropertyGetterFunc(objectID, statusType, &compileStatus); // 오류 여부를 3번째 인자에 넘긴다.
	//iv = integer vector
	if(compileStatus != GL_TRUE) // 비정상일 경우 GL_FALSE 반환.
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength]; // 오류 메시지(문자열)을 받을 버퍼.

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete []buffer;
		return false;
	}	

	return true;
}

//***** 쉐이더 컴파일 오류확인 코드 *****//
bool checkShaderStatus( GLuint shaderID ) 
{	
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//***** 프로그램 링크 오류확인 코드(쉐이터 컴파일 오류확인 코드와 거의 같다.) *****//
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
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);    // Vertex Shader Object 생성
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);// Fragment Shader Object 생성

	const char* adapter[1];
	
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0); // 1 = adapter 배열 크기, adapter = 배열
												   // 0 = ???
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// 컴파일에러 체크 //
	if( !checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID) )
		return;

	GLuint programID = glCreateProgram();		// 프로그램에 여러 컴파일 된 쉐이더를 포함시켜
												// 링크시키고, GPU에서 실행시킨다.
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// 링크에러 체크 //
	if( !checkProgramStatus(programID))
		return;

	glUseProgram(programID);
	
};

void MeGlWindow::initializeGL()
{
	glewInit(); // 초기화 하지 않으면 glew함수를 사용할 때 에러 발생

	glEnable(GL_DEPTH_TEST); // 먼저 그려지는 도형과 나중에 그려지는 도형의 depth정보를 비교해서
							 // 이미지가 겹치는 경우 depth가 높은 color를 colorBuffer에 저장한다.

	sendDataToOpenGL(); // 버퍼 생성, 버퍼에 좌표, 색상 데이터를 저장한다.
	installShader();
}