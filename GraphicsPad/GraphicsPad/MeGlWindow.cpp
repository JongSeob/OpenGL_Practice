#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include "MeGlWindow.h"

using namespace std;

#pragma comment(lib,"opengl32.lib") // glClearColor(), glClear 등의 함수를 사용할 때 이 구문을 넣지 않으면 링크 에러 발생

void sendDataToOpenGL() 
{
	// vertex = 꼭짓점
	GLfloat verts[] =    // 화면의 중심좌표 = (0, 0) 오른쪽 좌표 = (1, 0)  위쪽 좌표(0, 1)
	{
			+0.0f, +1.0f,   //                 index = 2
			+1.0f, +0.0f, +0.0f,
			-1.0f, -1.0f,   //                 index = 3
			+0.0f, +1.0f, +0.0f,
			+1.0f, -1.0f,   //                 index = 4
			+0.0f, +0.0f, +1.0f,

// 		+0.0f, +0.0f,   // 첫 번째 삼각형  index = 0
// 		+1.0f, +0.0f, +0.0f,  // 꼭짓점의 RGB 데이터
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
	*	꼭지점 좌표를 이용해 삼각형을 그리는 2가지 방식
	*
	*  1. ARRAY 버퍼에 모든 꼭짓점을 저장한 후 순차적으로 읽어 꼭지점을 구한다. (중복되는 꼭지점도 모두 적어줘야한다.)
	*  2. 1.에서 등록한 ARRAY버퍼에 인덱스로 접근해서 꼭지점을 구한다.			 (인데스로 접근하므로 중복되는 꼭지점은 한번만 정의해도 된다.)
	*/


	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); // GL_ARRAY_BUFFER 또는 GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), 
		verts, GL_STATIC_DRAW);

	

	/*
	 *	glEnableVertexAttribArray(0); 에서의 인자 0 과,
	 *  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); 에서의 첫 번째 인자 0은
	 *  둘 다 index를 의미한다. 이것이 Vertex 의 좌표와 색상을 구분짓게 만든다.
	 *
	 *  그리고 glVertexAttribPointer의 2,3 번째 인자는 해당 성분의 인자가 2개의 float으로 이루어져 있단 것을 뜻한다.
	 *
	 *  여기까지의 내용은 MeShaderCode.cpp 에서 "in layout(location=0) vec2 position;" 을 봤을 때
	 *  location=0의 숫자 0, vec2의 숫자 2와 매칭된다.
	 */

	// verts[] 배열에서 Vertex 좌표 위치
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);  // 5번째 인자 = strider = 포인터가 다음 인자를 가리킬 때 주소를 몇 증가시켜야 하는지에 대한 정보
	
	// verts[] 배열에서 Vertex의 색상 위치
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2)); // 6번째 인자 = 처음 가리키는 위치

	GLushort indices[] = {0,1,2};
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);
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

	sendDataToOpenGL(); // 버퍼 생성, 버퍼에 좌표, 색상 데이터를 저장한다.
	installShader();
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height()); // 도형의(삼각형의) 크기를 윈도우 크기에 딱 맞춘다.
	//glDrawArrays(GL_TRIANGLES, 0, 6);	 // 3번째 인자 = 꼭짓점 갯수

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	
	

	// 화면을 한가지 색상으로 칠한다.
	//glClearColor(0, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
}