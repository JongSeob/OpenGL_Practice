#ifndef MeGlWindow_h__
#define MeGlWindow_h__

#include <QtOpenGL/QGLWidget>

class MeGlWindow : public QGLWidget // QGLWidget - Qt 화면에 출력되는 Opengl Widget
{
protected:
	// virtual 함수들
	void initializeGL(); // 처음 paintGL()함수(또는 resizeGL()함수)가 실행되면 호출된다. 
	void paintGL();		 // Qt화면이 나타날 때마다(화면이 업데이트 될 때마다) 실행된다.
public:
};

#endif // MeGlWindow_h__
