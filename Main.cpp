#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <random>
#include <chrono>
#include <thread>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


struct color {
	float *red, *green, * blue;

	color(float* r, float* g, float* b) : red(r), green(g), blue(b) {
		// can use logic in constructor
		// constructors in cpp are apparently very different
		// i got confused why this->red(red) worked
		// went down the rabbit hole and found out that
		// member(variable) is the way to go. i renamed my
		// contructor parameters to make it easier to read.
		*r /= 255.0f;
		*g /= 255.0f;
		*b /= 255.0f;
		// honestly, i still need to research about this-> in constructor
		// oh and also -> is used for dereferncing [same as (*pointer_class).member ]
		// shit's weird
	}

};

// i totally COULD just pass by reference 
// and get the job done but i used a struct 
// now and im too deep into this rabbit hole
static void slowburn(color &var) {

	// change to randomness
	
	std::mt19937_64 randomgen{ static_cast<std::mt19937::result_type>(
		std::chrono::steady_clock::now().time_since_epoch().count()
		) };

	int lower = 50, upper = 150;

	std::uniform_int_distribution<> hex{ lower,upper };
	*var.red	= hex(randomgen) / 255.0f;
	*var.blue	= hex(randomgen) / 255.0f;
	*var.green  = hex(randomgen) / 255.0f;
	return (void)0;
}

int main() {
	// initialises glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);// tells whhich glfw versino we are using
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // and which profile (core or comaptiblity)

	GLfloat vertices[] = {
		-0.5f ,-0.5f , 0.0f,
		-0.5f , 0.5f , 0.0f,                          // for OpenGL the coordinates are normalised. leftmost for x is -1.0f 
		 0.5f ,-0.5f , 0.0f, 						     // right most is 1.0f. likewise with y-coordinates.
		 0.5f , 0.5f , 0.0f 
	};


	// object initialisation (width, height, name, )
	GLFWwindow* window_name = glfwCreateWindow(800, 600, "OPENGLTRIAL", NULL, NULL);
	
	glfwMakeContextCurrent(window_name);
	// [https://www.glfw.org/docs/latest/group__window.html#ga3c96d80d363e67d13a41b5d1821f3242]
	 //just making a window doesn't mean it will show. need to make the window context CURRENT. ye wala abhi ka hai<


	gladLoadGL();

	glViewport(0, 0, 800, 600);

	float r = 88.0f;//
	float g = 25.0f;// rgb-->(0-255)
	float b = 0.0f; //
	/*
	float *r = nullptr;
	float *g = nullptr;
	float *b = nullptr;
	*r = 0.0f;
	*g = 0.0f;
	*b = 0.0f;
	^^^^^^^^ 
	wtf is wrong with me i just need 
	to make the floats normally. 
	no need for them to be pointer
	it makes no sense bro
	i got so lost in the sauce that 
	i somehow thought of making 
	them into pointers. stoopiddd
	anyways

	*/
	
	color c = color(&r, &g, &b);

	// i did color c(r,g,b) 
	// i dont fucking know
	// what i was doing
	
	// okay so for the LONGEST time 
	// i was doing c.red, c->red, c.*red to access the pointer red variable 
	// but couldn't. the reason being struct_name.member acts as if you've 
	// directly called the member
	// struct.variable is equivalent to defining and 
	// using the variable outside the struct. so it 
	// kinda makes sense that c.red would be a &red type 
	// not *red


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);


	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);




	GLuint VAO = 0, VBO = 0;
	//vertexArray object and vertexBuffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window_name))
	{
		
		glClearColor(*c.red, *c.green, *c.blue, 1.0f); // glClearColor accepts 0-1.0f
		glClear(GL_COLOR_BUFFER_BIT);
		slowburn(c);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 1, 3);
		glfwSwapBuffers(window_name);
		glfwSwapInterval(-1);
		glfwPollEvents();

	}

	
	
	glfwDestroyWindow(window_name);
	glfwTerminate();
	return 0;
}












//													>>>>	DAY ONE		<<<<
//		What i learnt: 
//					1. How to make an OpenGL window, make the window context CURRENT to "activate" it. 
//					2. About OpenGL buffer(front and back), their swapping to display
//					3. glClearColor(*) and glClear(buffer_bit)
//					
//		What tinkering i did:
//					1. changed LINKER settings because the console was annoying me
//					2. explored glfw documentation about glfwGetAttribute, callbacks, and some more
//				
//		What i learnt, did again etc:
//					1. forgot pointer initialisation
//					2. learnt that structures can have constructors
//					3. passed a constructor by reference to a function
//					4. realised the function can be made static (idfk what the use is im stumped rn)
//					5. just look at Line 63



//													>>>>	DAY TWO		<<<<
//		What i learnt: 
//					1. 
//					
//		What tinkering i did:
//					1. made the color generate continuously and generate randomly
//					2. 
//				
//		What i learnt, did again etc:
//					1. use of <chrono> and <thread> libraries to use add delay/sleep; 
//					   the existence of chrono(time) literals using std::chrono::(seconds,milliseconds, hours etc)
//					2. 
//					




//													>>>>	DAY XYZ		<<<<
//		What i learnt: 
//					1. 
//					
//		What tinkering i did:
//					1. 
//				
//		What i learnt, did again etc:
//					1. 