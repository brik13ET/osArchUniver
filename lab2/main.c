#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <GL/glew.h>
#include <unistd.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "vec3.h"
#include "scene.h"

#define norm(x) ((x+1)/2.f)
#define VEC3_111 (vec3f){1.f,1.f,1.f}

typedef struct
{
	GLuint VBO, mode, count;
	vec3f color;
} fig;

vec3f scale;

void GLAPIENTRY
debug_callback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
	fprintf(
		stderr,
		"GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	    ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
	    type,
		severity,
		message
	);
	exit(0);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	printf("Window size: %dx%d\n", width, height);
	float max = (width >= height ? width : height);
	scale.x = width / max;
	scale.y = height / max;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void empty_callback()
{
	exit(0);
}

void signals_init()
{
	signal(SIGSEGV, empty_callback);
	signal(SIGINT, empty_callback);
}

GLFWwindow* window;
void glfw_init()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{

		fprintf(stderr, "glfwInit Err\n");
        exit(-1);
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    window = glfwCreateWindow(mode->width, mode->height, "Simple example",  glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
		fprintf(stderr, "window == NULL\n");
        exit(-1);
    }


	float max = (mode->width >= mode->height ? mode->width : mode->height);
	scale.x = mode->width / max;
	scale.y = mode->height / max;

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
    glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback(debug_callback, NULL);
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	char* VertexShaderCode;
	FILE* VertexShaderStream = fopen(vertex_file_path, "r");
	if(VertexShaderStream != NULL){
		fseek(VertexShaderStream, 0, SEEK_END);
		long fsize = ftell(VertexShaderStream);
		fseek(VertexShaderStream, 0, SEEK_SET);  /* same as rewind(f); */

		VertexShaderCode = malloc(fsize + 1);
		fread(VertexShaderCode, fsize, 1, VertexShaderStream);
		fclose(VertexShaderStream);
		VertexShaderCode[fsize] = 0;
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	char* FragmentShaderCode;
	FILE* FragmentShaderStream = fopen(fragment_file_path, "r");
	if(FragmentShaderStream != NULL){
		fseek(FragmentShaderStream, 0, SEEK_END);
		long fsize = ftell(FragmentShaderStream);
		fseek(FragmentShaderStream, 0, SEEK_SET);  /* same as rewind(f); */

		FragmentShaderCode = malloc(fsize + 1);
		fread(FragmentShaderCode, fsize, 1, FragmentShaderStream);
		fclose(FragmentShaderStream);
		FragmentShaderCode[fsize] = 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode;
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char VertexShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode;
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char FragmentShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char ProgramErrorMessage[InfoLogLength+1];
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	free(VertexShaderCode);
	free(FragmentShaderCode);

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

fig drawConvexPolygon(float cx, float cy, float r, int n, float offset, vec3f color)
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	float* vertices = malloc((n+1)*6*sizeof(float));

	vertices[n*6 + 0] = cx;
	vertices[n*6 + 1] = cy;
	vertices[n*6 + 2] = 0;
	vertices[n*6 + 3] = color.x;
	vertices[n*6 + 4] = color.y;
	vertices[n*6 + 5] = color.z;

	float A = (n - 2) * M_PI / n;
	float S = M_PI - A;
	float C = offset;
	for (size_t i = 0; i < n ; i++)
	{
		vertices[i*6 + 0] = cx + (r * cos(C) )/ scale.x;
		vertices[i*6 + 1] = cy + (r * sin(C) )/ scale.y;
		vertices[i*6 + 2] = scale.z;
		vertices[i*6 + 3] = color.x;
		vertices[i*6 + 4] = color.y;
		vertices[i*6 + 5] = color.z;
		C += S;
	}

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, (n+1) * 6 * sizeof(float), vertices, GL_STREAM_DRAW);
	free(vertices);
	return (fig){ .mode = GL_TRIANGLE_FAN, .VBO = vertexbuffer, .count = (n+1)};
}

fig drawCircle(float cx, float cy, float r, vec3f color)
{
	return drawConvexPolygon(cx, cy, r, 23, 0, color);
}

fig drawRect(float cx, float cy, float w, float h, vec3f color)
{
	w /= scale.x;
	h /= scale.y;

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	float vertices[] = {
		cx - w /2, cy - h /2, 0.0f, color.x, color.y, color.z,
		cx + w /2, cy - h /2, 0.0f, color.x, color.y, color.z,
		cx - w /2, cy + h /2, 0.0f, color.x, color.y, color.z,
		cx + w /2, cy + h /2, 0.0f, color.x, color.y, color.z
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	return (fig){ .mode = GL_TRIANGLE_STRIP, .VBO = vertexbuffer, .count = 4};
}

void draw(fig f, GLuint shader_program_handle)
{
	glBindBuffer(GL_ARRAY_BUFFER, f.VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,                  // count (x, y, 0)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float)*6,    // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,                  // count (r, g, b)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float)*6,    // stride
		(void*)(sizeof(float) * 3)// array buffer offset
	);

	glDrawArrays(f.mode, 0, f.count);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

 vec3f hsvToRgb(vec3f hsv){
	vec3f ret;

    int i = (int)(hsv.x * 6);
    float f = hsv.x * 6 - i;
    float p = hsv.z * (1 - hsv.y);
    float q = hsv.z * (1 - f * hsv.y);
    float t = hsv.z * (1 - (1 - f) * hsv.y);

    switch(i % 6){
        case 0: ret.x = hsv.z, ret.y = t, ret.z = p; break;
        case 1: ret.x = q, ret.y = hsv.z, ret.z = p; break;
        case 2: ret.x = p, ret.y = hsv.z, ret.z = t; break;
        case 3: ret.x = p, ret.y = q, ret.z = hsv.z; break;
        case 4: ret.x = t, ret.y = p, ret.z = hsv.z; break;
        case 5: ret.x = hsv.z, ret.y = p, ret.z = q; break;
    }

    return ret;
}

int main(int, char**)
{
	signals_init();
	putenv("DISPLAY=:0");

	glfw_init();

	GLuint prog =  LoadShaders("./vertex.glsl","./frag.glsl");

	fig figs[5];
	// 350/350 185/350

	fig drawlist[sizeof(scene)/sizeof(scene[0])];

	for (size_t i = 0; i < sizeof(scene)/sizeof(scene[0]); i++)
	{
		if (scene[i].type == 'c')
			drawlist[i] = drawCircle(scene[i].c_pos.x, scene[i].c_pos.y, scene[i].data[0], scene[i].color);
		else if (scene[i].type == 'r')
			drawlist[i] = drawRect(scene[i].c_pos.x, scene[i].c_pos.y, scene[i].data[0], scene[i].data[1], scene[i].color);
	}


	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();
		// double time = 0;
		// scale.z = sin(time);
		vec3f hsv = { .x = norm(sin(time/10)), .y = 0.5f, .z = 0.2f};
		vec3f rgb = hsvToRgb(hsv);
		glClearColor(rgb.x, rgb.y, rgb.z, 255);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(prog);

		int _i = (sizeof(scene)/sizeof(scene[0])) - 1;
		drawlist[_i] = drawRect(scene[_i].c_pos.x  - 0.2f +sin(time) * 0.2f, scene[_i].c_pos.y, scene[_i].data[0], scene[_i].data[1], scene[_i].color);

		for (size_t i = 0; i < (sizeof(scene)/sizeof(scene[0])); i++)
		{
			draw(drawlist[i], prog);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
