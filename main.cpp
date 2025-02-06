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
#include "model.h"
#define global_variable static
std::vector<Texture> textures_loaded;
global_variable Model backpackModel;
global_variable Shader crossShader;
global_variable Shader modelShader;

global_variable glm::mat4 trans;
global_variable glm::mat4 view;
global_variable glm::mat4 model;
global_variable glm::mat4 projection;

global_variable glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

global_variable Model megalodon;

global_variable Camera camera;


global_variable bool firstMouse = true;


global_variable float lastX = 720;
global_variable float lastY = 540;


global_variable float deltaTime;
global_variable float lastFrame;

global_variable unsigned int crosshairVAO, crosshairVBO;


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

void CreateShader()
{
	ReadAndCompileShader(&modelShader, "model.vs", "model.frag");
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


	UseShader(&modelShader);
	CreateModelMatrix();
	CreateViewMatrix();
	CreateProjectionMatrix();
	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));



	///render model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	



	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	DrawModel(&backpackModel, modelShader);


	//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));


	//DrawModel(&megalodon, modelShader);




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

	CreateModelMatrix();
	CreateViewMatrix();
	CreateProjectionMatrix();

	transformation();


	InitCamera(&camera);

	CreateCrosshairBuffers();


	stbi_set_flip_vertically_on_load(true);
	LoadModel(&backpackModel, "models/backpack/backpack.obj");

	//LoadModel(&megalodon, "models/whiteshark/scene.gltf");



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
	glfwTerminate();
	return 0;
}
