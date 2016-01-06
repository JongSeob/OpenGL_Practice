#include <gl/glew.h>
#include "MeGlWindow.h"
#include <iostream>

using namespace std;
#pragma comment(lib,"opengl32.lib") // glClearColor(), glClear ���� �Լ��� ����� �� �� ������ ���� ������ ��ũ ���� �߻�

void MeGlWindow::initializeGL()
{
	glewInit(); // �ʱ�ȭ ���� ������ glew�Լ��� ����� �� ���� �߻�

	// vertex = ������
	GLfloat verts[] =    // ȭ���� �߽���ǥ = (0, 0) ������ ��ǥ = (1, 0)  ���� ��ǥ(0, 1)
	{
		+0.0f, +0.0f,   // ù ��° �ﰢ��  index = 0
		+1.0f, +1.0f,   //                 index = 1
		-1.0f, +1.0f,   //                 index = 2

		//+0.0f, +0.0f,	// �� ��° �ﰢ��
		-1.0f, -1.0f,   //                 index = 3
		+1.0f, -1.0f,   //                 index = 4
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
	glViewport(0, 0, width(), height()); // ������(�ﰢ����) ũ�⸦ ������ ũ�⿡ �� �����.
	//glDrawArrays(GL_TRIANGLES, 0, 6);	 // 3��° ���� = ������ ����

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	

	// ȭ���� �Ѱ��� �������� ĥ�Ѵ�.
	//glClearColor(0, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
}