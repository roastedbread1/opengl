#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include "texture.h"
#include "camera.h"

#define global_variable static



global_variable unsigned int vbo;
global_variable int vertexShader;
global_variable unsigned int fragmentShader;
global_variable int shaderProgram;
global_variable unsigned int vao;
global_variable unsigned int ebo;

global_variable unsigned int cubeVAO;
global_variable unsigned int lightCubeVAO;

global_variable Shader lightningShader;
global_variable Shader lightCube;
global_variable Shader crossShader;

global_variable glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

global_variable Image image1;
global_variable Image image2;
global_variable glm::mat4 trans;
global_variable glm::mat4 view;
global_variable glm::mat4 model;
global_variable glm::mat4 projection;


global_variable Camera camera;



//global_variable glm::vec3 cameraTarget;
//global_variable glm::vec3 cameraDirection;
//global_variable glm::vec3 up;
//global_variable glm::vec3 cameraRight;
//global_variable glm::vec3 cameraUp;



global_variable bool firstMouse = true;


global_variable float lastX = 720;
global_variable float lastY = 540;


global_variable float deltaTime;
global_variable float lastFrame;


global_variable unsigned int texture1;
global_variable unsigned int texture2;
global_variable unsigned int crosshairVAO, crosshairVBO;

global_variable float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

global_variable float crosshairVertices[] = {
	// Horizontal line (x, y)
	-0.02f,  0.0f,   // Left
	 0.02f,  0.0f,   // Right

	 // Vertical line (x, y)
	  0.0f, -0.02f,   // Bottom
	  0.0f,  0.02f    // Top
};



void CreateCrosshairBuffers() {
	glGenVertexArrays(1, &crosshairVAO);
	glGenBuffers(1, &crosshairVBO);

	glBindVertexArray(crosshairVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

	// Position attribute (2 components now)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}


global_variable glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		ProcessKeyboard(&camera, FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		ProcessKeyboard(&camera, BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		ProcessKeyboard(&camera, LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		ProcessKeyboard(&camera, RIGHT, deltaTime);
	}
	//fps style camera
	//camera.position.y = 0.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sens = 0.1f;
	ProcessMouseMovement(&camera, xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ProcessMouseScroll(&camera, yoffset);
}

void CreateVertexBuffer()
{
	glGenBuffers(1, &vbo);
	///binding buffer to specify the type
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	///copy the vertex data in vertices variable to the vbo
	///glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
void CreateShader()
{
	ReadAndCompileShader(&lightningShader, "shader.vs", "shader.fs");
	ReadAndCompileShader(&crossShader, "crosshair.vs", "crosshair.fs");
	ReadAndCompileShader(&lightCube, "lightning.vs", "lightning.fs");
}




void CreateVertexAttributes()
{
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	////texture attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
}

void CreateVertexArrayObject()
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

}

void CreateElementBufferObject()
	{
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);
	}

void CreateCube()
{

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//normal vector
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


void CreateLightCube()
{

	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void transformation()
{
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (GLfloat)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

}

void CreateModelMatrix()
{
	model = glm::mat4(1.0f);
	/*model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/
}
void CreateViewMatrix()
{

	view = glm::mat4(1.0f);
	view = GetViewMatrix(&camera);

}

void CreateProjectionMatrix()
{
	//sending the update fov each frame if zooming in or zooming out
	projection = glm::perspective(glm::radians(camera.zoom), 1440.0f / 1080.0f, 0.1f, 100.0f);
}


void RenderScene()
{


	//glClearColor(0.5f, 0.2f, 0.8f, 1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgram(shaderProgram);



	UseShader(&lightningShader);
	glUniform3f(glGetUniformLocation(lightningShader.ID, "objectColor"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(lightningShader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(lightningShader.ID, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(lightningShader.ID, "viewPos"), 1, glm::value_ptr(camera.position));

	CreateModelMatrix();
	CreateViewMatrix();
	CreateProjectionMatrix();




	int viewLoc = glGetUniformLocation(lightningShader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(lightningShader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(lightningShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	///render cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	///render light cube
	UseShader(&lightCube);
	projLoc = glGetUniformLocation(lightCube.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	viewLoc = glGetUniformLocation(lightCube.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(lightCube.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	


	glBindVertexArray(lightCubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	///render crosshair
	UseShader(&crossShader);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(crosshairVAO);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);


	/*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
	glBindVertexArray(0);

}
int main()
{

	glfwInit();
	///first argument is an enum (many options), second is an interger that sets the value 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	///doesnt use the unnecessary backwards compatible features that we dont need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(1440, 1080, "breadman", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	///glad manages or abstracts function pointers of opengl for different OS
	///passing in glfwgetprocaddress to get the specific function loader for the current OS, basically just getting wglGetProcAddress (I think)
	/// and then queries all the available extensions and then queries the opengl function pointers (I think this is how it works im not sure)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD\n";
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 1440, 1080);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	CreateShader();
	//CreateVertexArrayObject();
	/*CreateVertexBuffer();*/
	//CreateElementBufferObject();
	/*CreateVertexAttributes();*/
	CreateCube();

	UseShader(&lightningShader);

	CreateModelMatrix();
	CreateViewMatrix();
	CreateProjectionMatrix();

	int projectionLoc = glGetUniformLocation(lightningShader.ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	transformation();


	InitCamera(&camera);

	CreateCrosshairBuffers();
	CreateLightCube();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		ProcessInput(window);
		RenderScene();
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glfwTerminate();
	return 0;
}
