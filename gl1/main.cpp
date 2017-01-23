#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SHADER.h"
#include "Camera.h"
#include "box.h"
#include "hammerCylinder.h"
#include "hammerHead.h"
#include "item1.h"
#include "light.h"
#include "triangles.h"
#include "buttons.h"

#define HAMMERSPEED 0.7

#define LIGHT_POSITION 3.2f, 4.0f, -1.0f
#define LIGHT_AMBIENT 0.1f, 0.1f, 0.1f
#define LIGHT_DIFFUSE 0.5f, 0.5f, 0.5f
#define LIGHT_SPECULAR 0.4f, 0.5f, 0.44f
#define LIGHT_COLOR 1.0f, 1.0f, 1.0f
#define CAMERA_POSITION 0.0f, 5.0f, 4.0f

#define HAMMER_SPECULAR_IMPACT 0.1f, 0.1f, 0.1f			//IMPACT = SPECULAR_COLOR
#define HAMMER_SHINNIESS 32.0f							//SHINNIESS = TYPE OF SPECULAR LIGHT
#define HAMMER_HEAD_SPECULAR_IMPACT 0.3f, 0.4f, 0.32f
#define HAMMER_HEAD_SHINNIESS 16.0f
#define TRIANGLES_SPECULAR_IMPACT 0.5f, 0.5f, 0.5f
#define TRIANGLES_SHINNIESS 64.0f
#define BOX_SPECULAR_IMPACT 0.4f, 0.4f, 0.6f
#define BOX_SHINNIESS 64.0f
#define MOVING_BOX_SPECULAR_IMPACT 0.2f, 0.6f, 0.48f
#define MOVING_BOX_SHINNIESS 256.0f
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
// global variables for parameters manipulation
glm::vec3 hammerSpecularImpact(HAMMER_SPECULAR_IMPACT);
glm::vec3 hammerHeadSpecularImpact(HAMMER_HEAD_SPECULAR_IMPACT);
glm::vec3 trianglesSpecularImpact(TRIANGLES_SPECULAR_IMPACT);
glm::vec3 boxSpecularImpact(BOX_SPECULAR_IMPACT);
glm::vec3 movingBoxSpecularImpact(MOVING_BOX_SPECULAR_IMPACT);

glm::vec3 lightPos(LIGHT_POSITION);
glm::vec3 light_ambient(LIGHT_AMBIENT);
glm::vec3 light_diffuse(LIGHT_DIFFUSE);
glm::vec3 light_specular(LIGHT_SPECULAR);
glm::vec3 light_color(LIGHT_COLOR);
Camera camera(glm::vec3(CAMERA_POSITION));

//window parameters
const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0;
//keys
GLboolean firstMouse = true;
bool keys[1024];
//time variables
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//openGL ver3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);			//init
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);			//using scroll, mouse and WSAD
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Building shaders 
	Shader ourShader("shader.vs", "shader.frag");
	Shader lampShader("lampShader.vertex", "lampShader.frag");

	//VAO AND VBO INIT
	GLuint hammerCylinderVBO, hammerCylinderVAO, hammerHeadVAO, hammerHeadVBO, trianglesVAO, trianglesVBO, item1VAO, item1VBO, boxVAO, boxVBO;
	GLuint lightVAO, lightVBO, buttonsVAO, buttonsVBO;
	glGenVertexArrays(1, &hammerCylinderVAO);
	glGenVertexArrays(1, &hammerHeadVAO);
	glGenVertexArrays(1, &trianglesVAO);
	glGenVertexArrays(1, &item1VAO);
	glGenVertexArrays(1, &boxVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenVertexArrays(1, &buttonsVAO);

	glGenBuffers(1, &buttonsVBO);
	glGenBuffers(1, &lightVBO);
	glGenBuffers(1, &boxVBO);
	glGenBuffers(1, &item1VBO);
	glGenBuffers(1, &hammerCylinderVBO);
	glGenBuffers(1, &hammerHeadVBO);
	glGenBuffers(1, &trianglesVBO);

	glBindVertexArray(hammerCylinderVAO);						//ourShader	(x, y, z, x_col, y_col, z_col, tx, ty, x_norm, y_norm, z_norm, )
	glBindBuffer(GL_ARRAY_BUFFER, hammerCylinderVBO);			//			 layout 0,   l    layout 1,   layout 2,      layout 3              
	glBufferData(GL_ARRAY_BUFFER, sizeof(hammerCylinder), hammerCylinder, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	glBindVertexArray(hammerHeadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, hammerHeadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hammerHead), hammerHead, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	glBindVertexArray(trianglesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	glBindVertexArray(item1VAO);
	glBindBuffer(GL_ARRAY_BUFFER, item1VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(item1), item1, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	glBindVertexArray(buttonsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, buttonsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttons), buttons, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light), light, GL_STATIC_DRAW);;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint texture1;
	GLuint texture2;
	GLuint texture4;
	GLuint texture5;
																//load 4 textures
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wood.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("black.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("wood3.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("grey.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	int i = 0;
	int j = 0;
	int k = 0;
	int speed = 1;
	float itemspeed =2;
	// Game loop

	glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 0);

	while (!glfwWindowShouldClose(window))
	{
		++i;
		if (i == 100) {						//used to control hammermovement
			i = 0;
			j += speed;
			if (abs(j % 6) == 0) {
				k += itemspeed;
				if (k == 6)
					itemspeed = -1;
				if (k == 0)
					itemspeed = 1;
			}
			if (j <= 0)
				speed = 1;
			if (j * HAMMERSPEED >= 90) {
				speed = (-1);
			}
		}

		glfwPollEvents();
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		do_movement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 0);			//setting shader uniforms and uniforms in structs

		GLint lightColorLoc = glGetUniformLocation(ourShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");

		GLint matAmbientLoc = glGetUniformLocation(ourShader.Program, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(ourShader.Program, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(ourShader.Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(ourShader.Program, "material.shininess");

		GLint lightAmbientLoc = glGetUniformLocation(ourShader.Program, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(ourShader.Program, "light.specular");
	

		glUniform3f(glGetUniformLocation(ourShader.Program, "light.ambient"), light_ambient.x, light_ambient.y, light_ambient.z);
		glUniform3f(glGetUniformLocation(ourShader.Program, "light.diffuse"), light_diffuse.x, light_diffuse.y, light_diffuse.z);
		glUniform3f(glGetUniformLocation(ourShader.Program, "light.specular"), light_specular.x, light_specular.y, light_specular.z);

		glUniform3f(lightColorLoc, light_color.x, light_color.y, light_color.z);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		glUniform3f(glGetUniformLocation(ourShader.Program, "material.specular"), hammerSpecularImpact.x, hammerSpecularImpact.y, hammerSpecularImpact.z);
		glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), HAMMER_SHINNIESS);

		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;
		GLfloat radius = 10.0f;
		view = camera.GetViewMatrix();

		model = glm::rotate(model, glm::radians((float)(HAMMERSPEED) * (j)), glm::vec3(0.0f, 0.0f, -1.0f));
		projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(hammerCylinderVAO);
		glDrawArrays(GL_TRIANGLES, 0, 216);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 1);
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.specular"), hammerHeadSpecularImpact.x, hammerHeadSpecularImpact.y, hammerHeadSpecularImpact.z);
		glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), HAMMER_HEAD_SHINNIESS);
		glBindVertexArray(hammerHeadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		model = glm::mat4();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 2);
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.specular"), trianglesSpecularImpact.x, trianglesSpecularImpact.y, trianglesSpecularImpact.z);
		glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), TRIANGLES_SHINNIESS);
		glBindVertexArray(trianglesVAO);
		glDrawArrays(GL_TRIANGLES, 0, 48);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 5);
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.specular"), boxSpecularImpact.x, boxSpecularImpact.y, boxSpecularImpact.z);
		glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), BOX_SHINNIESS);
		glBindVertexArray(boxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::translate(model, glm::vec3(-0.5f, 0.3f, -6.2 + (float)( 0.1*HAMMERSPEED*j)));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.specular"), movingBoxSpecularImpact.x, movingBoxSpecularImpact.y, movingBoxSpecularImpact.z);
		glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), MOVING_BOX_SHINNIESS);
		glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 0);
		glBindVertexArray(item1VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE5);
		glUniform1i(glGetUniformLocation(ourShader.Program, "material.diffuse"), 5);
		glBindVertexArray(buttonsVAO);
		glDrawArrays(GL_TRIANGLES, 0, 168);
		glBindVertexArray(0);



		lampShader.Use();
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		lightColorLoc = glGetUniformLocation(lampShader.Program, "lightColor");
		GLint objectColorLoc = glGetUniformLocation(lampShader.Program, "objectColor");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(lightColorLoc, light_color.x, light_color.y, light_color.z);
		glUniform3f(objectColorLoc, light_color.x, light_color.y, light_color.z);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glFlush();

		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &hammerCylinderVAO);
	glDeleteBuffers(1, &hammerCylinderVBO);
	glDeleteVertexArrays(1, &hammerHeadVAO);
	glDeleteBuffers(1, &hammerHeadVBO);
	glDeleteVertexArrays(1, &trianglesVAO);
	glDeleteBuffers(1, &trianglesVBO);
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteBuffers(1, &boxVBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteVertexArrays(1, &buttonsVAO);
	glDeleteBuffers(1, &buttonsVBO);
	glDeleteVertexArrays(1, &item1VAO);
	glDeleteBuffers(1, &item1VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void do_movement()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	//	if (keys[GLFW_KEY_SPACE])
	//	glfwSetTime
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(glm::radians(yoffset));
}
