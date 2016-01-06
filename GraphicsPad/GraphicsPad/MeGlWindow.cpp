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
		+0.0f, +0.0f,   // 첫 번째 삼각형  index = 0
		+1.0f, +1.0f,   //                 index = 1
		-1.0f, +1.0f,   //                 index = 2

		//+0.0f, +0.0f,	// 두 번째 삼각형
		-1.0f, -1.0f,   //                 index = 3
		+1.0f, -1.0f,   //                 index = 4
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLushort indices[] = {0,1,2,  0,3,4};
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height()); // 도형의(삼각형의) 크기를 윈도우 크기에 딱 맞춘다.
	//glDrawArrays(GL_TRIANGLES, 0, 6);	 // 3번째 인자 = 꼭짓점 갯수

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	

	// 화면을 한가지 색상으로 칠한다.
	//glClearColor(0, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
}