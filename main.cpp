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
global_variable Shader ourShader;
global_variable Shader crossShader;
global_variable Image image1;
global_variable Image image2;
global_variable glm::mat4 trans;
global_variable glm::mat4 view;
global_variable glm::mat4 model;
global_variable glm::mat4 projection;

//make it a struct!! 
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

global_variable float vertices[] =
	{
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // Changed to 2
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
global_variable int indices[] =
	{
		//0, 1, 2,
		//0, 3, 4
		0, 1, 3,
		1, 2, 3
	};

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
	camera.position.y = 0.0f;
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
	ReadAndCompileShader(&ourShader, "shader.vs", "shader.fs");
	ReadAndCompileShader(&crossShader, "crosshair.vs", "crosshair.fs");
}




void CreateVertexAttributes()
{
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);
	}

void transformation()
{
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (GLfloat)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	//model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
}

void CreateModelMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
}
void CreateViewMatrix()
{
	//position, origin, up
	//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::mat4(1.0f);
	//const float radius = 10.0f;
	//float camX = sin(glfwGetTime() ) * radius;
	//float camZ = cos(glfwGetTime() ) * radius;


	//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = GetViewMatrix(&camera);

}

void CreateProjectionMatrix()
{
	//sending the update fov each frame if zooming in or zooming out
	projection = glm::perspective(glm::radians(camera.zoom), 1440.0f / 1080.0f, 0.1f, 100.0f);
}

//idk if I want too keep this
//void CreateCamera()
//{
//	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//	//inverse direction actually
//	cameraDirection = glm::normalize(cameraPos - cameraTarget);
//
//	up = glm::vec3(0.0f, 1.0f, 0.0f);
//	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//
//	cameraUp = glm::cross(cameraDirection, cameraRight );
//
//}

void RenderScene()
{
	//static GLclampf c = 0.0f;
	glClearColor(0.5f, 0.2f, 0.8f, 1.0f);

	//c += 1.0f / 256.0f;
	//if (c >= 1.0f)
	//{
	//	c = 0.0f;
	//}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgram(shaderProgram);

	/*color changing */
	float timeValue = glfwGetTime();
	float blueValue = (sin(timeValue) / 2.0f) + 0.5f;

	UseShader(&ourShader);
	/*SetShaderFloat(&ourShader,"ourColor", 1.0f);*/

	transformation();
	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");


	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	CreateModelMatrix();
	CreateViewMatrix();
	CreateProjectionMatrix();


	int viewLoc = glGetUniformLocation(ourShader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(ourShader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glBindVertexArray(vao);



	for (unsigned int i = 0; i < 10; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
	}

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
	



	LoadImage(&image1, "container.jpg");
	CreateTexture(&image1, &texture1, 1, GL_RGB);
	LoadImage(&image2, "Peter_Griffin.png");
	CreateTexture(&image2, &texture2, 1, GL_RGBA);
	


	CreateShader();
	CreateVertexArrayObject();
	CreateVertexBuffer();
	CreateElementBufferObject();
	CreateVertexAttributes();




	
	UseShader(&ourShader);
	SetShaderInt(&ourShader, "texture1", 0);
	SetShaderInt(&ourShader,"texture2", 1);

	CreateModelMatrix();
	CreateViewMatrix();
	CreateProjectionMatrix();

	int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	transformation();


	InitCamera(&camera);

	CreateCrosshairBuffers();
	

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
