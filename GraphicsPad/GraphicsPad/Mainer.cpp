#include <iostream>
#include <QtWidgets/QApplication>
#include "MeGlWindow.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////
// 에러 발생.  gl.h included before glew.h								//
// <QtOpenGL/QGLWidget> 가 glew보다 나중에 선언되야한다.				//
// #include "MeGlWindow.h"												//
// #include <gl/glew.h>													//
//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	// OpenGl이 Qt를 이용하기 때문에 QApplication 객체를 생성하지 않으면(생성자가 실행되지 않으면)
	// 런타임에러가 발생한다.

	QApplication app(argc, argv);
	MeGlWindow meWindow;

	meWindow.show();
	
	return app.exec();
}

