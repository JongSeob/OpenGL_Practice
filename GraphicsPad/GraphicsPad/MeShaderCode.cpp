
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
	"#version 430\r\n"  // 오래된 그래픽카드의 경우 이 문장에서 에러가 발생할 수 있다.
	"\n"
	"in layout(location=0) vec2 position;\n"    // vertexShader로 들어가는(in) 좌표를 뜻함
	"in layout(location=1) vec3 vertexColor;\n" // MeGlWindow.cpp 의 sendDataToOpenGL() 참조.
	"\n"
	"out vec3 theColor;\n"
	"\n"
	"void main()\n"							  
	"{\n"
	"	gl_Position = vec4(position, 0.0, 1.0);\n" // OpenGL로 부터 얻은 좌표를 vertexShader 위치정보에 포함한다.
	"   theColor = vertexColor;\n"
	"}\n";										 // 좌표는 각각 (x, y, z, w) 가 된다. 현재 2차원 형태의 삼각형을 그리기 때문에 z는 0이고,
												 // 4번째 w는 homogeneous coordinate(동차좌표)에서 언급 되는데
												 // w가 1이면 x,y,z는 위치좌표, w가 0이면 x,y,z는 방향좌표를 의미하게 된다.
												 // w가 없으면 3차원 좌표가 위치좌표인지, 방향좌표인지 구분할 수 없으므로 구분을 위해서
												 // 추가적으로 삽입한 좌표이다. 

const char* fragmentShaderCode =
// 	"void main (void)  "
// "{     "
// 	"gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  "
// "}     ";
	"#version 430\r\n\n"
	"\n"
	"out vec4 daColor;\n" // fragmentShader에서 나가는(out) rgb데이터를 뜻함
	"in vec3 theColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	daColor = vec4(theColor, 1.0);\n"
	"}\n";
