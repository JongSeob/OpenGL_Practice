#include <iostream>
#include <QtWidgets/QApplication>
#include "MeGlWindow.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////
// ���� �߻�.  gl.h included before glew.h								//
// <QtOpenGL/QGLWidget> �� glew���� ���߿� ����Ǿ��Ѵ�.				//
// #include "MeGlWindow.h"												//
// #include <gl/glew.h>													//
//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	// OpenGl�� Qt�� �̿��ϱ� ������ QApplication ��ü�� �������� ������(�����ڰ� ������� ������)
	// ��Ÿ�ӿ����� �߻��Ѵ�.

	QApplication app(argc, argv);
	MeGlWindow meWindow;

	meWindow.show();
	
	return app.exec();
}

