
const char* vertexShaderCode = 
	"#version 430/r/n"  // ������ �׷���ī���� ��� �� ���忡�� ������ �߻��� �� �ִ�.
	""
	"in layout(location=0) vec2 position;" // vertexShader�� ����(in) ��ǥ�� ����
	""
	"void main()"
	"{"
	"	gl_Position = vec4(position, 0.0, 1.0);" // OpenGL�� ���� ���� ��ǥ�� vertexShader ��ġ������ �����Ѵ�.
	"}";										 // ��ǥ�� ���� (x, y, z, w) �� �ȴ�. ���� 2���� ������ �ﰢ���� �׸��� ������ z�� 0�̰�,
												 // 4��° w�� homogeneous coordinate(������ǥ)���� ��� �Ǵµ�
												 // w�� 1�̸� x,y,z�� ��ġ��ǥ, w�� 0�̸� x,y,z�� ������ǥ�� �ǹ��ϰ� �ȴ�.
												 // w�� ������ 3���� ��ǥ�� ��ġ��ǥ����, ������ǥ���� ������ �� �����Ƿ� ������ ���ؼ�
												 // �߰������� ������ ��ǥ�̴�. 

const char* fragmentShaderCode =
	"#version 430/r/n"
	""
	"out vec4 daColor" // fragmentShader���� ������(out) rgb�����͸� ����
	""
	"void main()"
	""
	"{"
	"	daColor = vec4(0.0, 1.0, 0.0, 1.0);"
	"}";
