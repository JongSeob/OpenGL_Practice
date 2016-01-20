
const char* vertexShaderCode = 
// 	"void main(void)"
// "{"
// 	"vec4 a = gl_Vertex;"
// 	"a.x = a.x * 0.5;"
// 	"a.y = a.y * 0.5;"
// 
// 
// 	"gl_Position = gl_ModelViewProjectionMatrix * a;"
// 
// "}";       
	"#version 430\r\n"  // ������ �׷���ī���� ��� �� ���忡�� ������ �߻��� �� �ִ�.
	"\n"
	"in layout(location=0) vec2 position;\n"    // vertexShader�� ����(in) ��ǥ�� ����
	"in layout(location=1) vec3 vertexColor;\n" // MeGlWindow.cpp �� sendDataToOpenGL() ����.
	"\n"
	"out vec3 theColor;\n"
	"\n"
	"void main()\n"							  
	"{\n"
	"	gl_Position = vec4(position, 0.0, 1.0);\n" // OpenGL�� ���� ���� ��ǥ�� vertexShader ��ġ������ �����Ѵ�.
	"   theColor = vertexColor;\n"
	"}\n";										 // ��ǥ�� ���� (x, y, z, w) �� �ȴ�. ���� 2���� ������ �ﰢ���� �׸��� ������ z�� 0�̰�,
												 // 4��° w�� homogeneous coordinate(������ǥ)���� ��� �Ǵµ�
												 // w�� 1�̸� x,y,z�� ��ġ��ǥ, w�� 0�̸� x,y,z�� ������ǥ�� �ǹ��ϰ� �ȴ�.
												 // w�� ������ 3���� ��ǥ�� ��ġ��ǥ����, ������ǥ���� ������ �� �����Ƿ� ������ ���ؼ�
												 // �߰������� ������ ��ǥ�̴�. 

const char* fragmentShaderCode =
// 	"void main (void)  "
// "{     "
// 	"gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  "
// "}     ";
	"#version 430\r\n\n"
	"\n"
	"out vec4 daColor;\n" // fragmentShader���� ������(out) rgb�����͸� ����
	"in vec3 theColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	daColor = vec4(theColor, 1.0);\n"
	"}\n";
