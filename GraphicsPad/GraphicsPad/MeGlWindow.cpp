#include <gl/glew.h>
#include "MeGlWindow.h"
#include <iostream>

using namespace std;
#pragma comment(lib,"opengl32.lib") // glClearColor(), glClear 등의 함수를 사용할 때 이 구문을 넣지 않으면 링크 에러 발생

void MeGlWindow::initializeGL()
{
	glewInit(); // 초기화 하지 않으면 glew함수를 사용할 때 에러 발생

	// vertex = 꼭짓점
	GLfloat verts[] =    // 화면의 중심좌표 = (0, 0) 오른쪽 좌표 = (1, 0)  위쪽 좌표(0, 1)
	{
		+0.0f, +0.0f,   // 첫 번째 삼각형
		+1.0f, +1.0f,
		-1.0f, +1.0f,

		+0.0f, +0.0f,	// 두 번째 삼각형
		-1.0f, -1.0f,
		+1.0f, -1.0f,
	};

	// 내부에 Buffer Object를 생성
	GLuint myBufferID;
	glGenBuffers(1, &myBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID); // GL_ARRAY_BUFFER 또는 GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), 
		verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);	
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height()); // 도형의(삼각형의) 크기를 윈도우 크기에 딱 맞춘다.
	glDrawArrays(GL_TRIANGLES, 0, 6);	 // 3번째 인자 = 꼭짓점 갯수
	
	

	// 화면을 한가지 색상으로 칠한다.
	//glClearColor(0, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
}