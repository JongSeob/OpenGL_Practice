#ifndef MeGlWindow_h__
#define MeGlWindow_h__

#include <QtOpenGL/QGLWidget>

class MeGlWindow : public QGLWidget // QGLWidget - Qt ȭ�鿡 ��µǴ� Opengl Widget
{
protected:
	// virtual �Լ���
	void initializeGL(); // ó�� paintGL()�Լ�(�Ǵ� resizeGL()�Լ�)�� ����Ǹ� ȣ��ȴ�. 
	void paintGL();		 // Qtȭ���� ��Ÿ�� ������(ȭ���� ������Ʈ �� ������) ����ȴ�.
public:
};

#endif // MeGlWindow_h__
