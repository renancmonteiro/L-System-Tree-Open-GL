/*
	made by Renan Monteiro <renanmonteiro@msn.com>
	any questions do not hesitate to contact me
	thank you.
*/

/* ----------------------------------------------------------------

	A tree is a line from root point to tree top point followed by 
	two sub-trees, each having the root at the top of parent tree 
	and own top at new calculated point.
   ----------------------------------------------------------------- */


#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define N 15 // number max of interations
#define ANGLE 35.0 // angle of right and left sub tree
#define treeRatio 0.2f // scale of sub tree according parent tree

int n = 1;

// dimensions of window
const GLint WIDTH = 1024, HEIGHT = 768;

using namespace std;

void initialize(){ 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
        if (n > 0){
			n--;
		}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		if(n < N){
			n++;
		}
}

void arvore(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat length, int depth, GLint VAO, GLint VBO)
{
		// stop de recursion
		if (depth < 1)	return;

		GLfloat vertex_data[] = {
			x1, y1, 0.0f,
			x2, y2, 0.0f,
		};

		// draw the line
		glBindVertexArray(VAO);
		glLineWidth(1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);


		// calculate position of new pointer
		float x3 = (x2-x1)*treeRatio+x1-x2;
		float y3 = (y2-y1)*treeRatio+y1-y2;
		
		// right sub tree
		arvore(x2, y2, x3 * cos(ANGLE) + y3 * sin(ANGLE) + x2, -x3 * sin(ANGLE) + y3 * cos(ANGLE) + y2, length, depth-1, VAO, VBO);
		
		// left sub tree
		arvore(x2, y2, x3 * cos(-ANGLE) + y3 * sin(-ANGLE) + x2, -x3 * sin(-ANGLE) + y3 * cos(-ANGLE) + y2, length, depth-1, VAO, VBO);
}

int main()
{
	// Initialize GLFW
	if(!glfwInit())
	{
		cerr << "Failed to initialize GLFW" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tree", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." << endl;
		glfwTerminate();
		return -1;
	}

	glewExperimental = GL_TRUE; 
	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		cerr << "Failed to initialize GLEW\n" << endl;
		return -1;
	}
	
	// Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
	Shader ourShader("shader.vs", "shader.frag");

	glewExperimental = GL_TRUE;

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	initialize();

	// Get a handle for our buffers
	GLuint vCoordID = glGetAttribLocation(ourShader.Program, "vCoord");
	//GLuint TID = glGetUniformLocation(ourShader.Program, "T");
			
	//Criação dos buffers
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	
	glfwSetKeyCallback(window, keyCallback);
	do
	{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		ourShader.Use();
		
		// draw the tree
		arvore(0,-1, 0, -0.6, 0.8,n,VAO,VBO);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);


	// Cleanup VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(ourShader.Program);

	// Close OpenGL window and terminate GLFW
	// glfwTerminate();
	return 0;
}

