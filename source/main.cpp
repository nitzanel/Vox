#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/* Create a windowed mode window and its OpenGL context */
	int windowWidth = 800;
	int windowHeight = 800;
	window = glfwCreateWindow(windowWidth, windowHeight, "Vox", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Center on screen */
	int width;
	int height;
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwGetWindowSize(window, &width, &height);
	glfwSetWindowPos(window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Create the renderer */
	Renderer* pRenderer = new Renderer(windowWidth, windowHeight, 32, 8);

	// Create game viewport
	unsigned int m_defaultViewport;
	pRenderer->CreateViewport(0, 0, windowWidth, windowHeight, 60.0f, &m_defaultViewport);

	/* Create fonts */
	unsigned int m_defaultFont;
	pRenderer->CreateFreeTypeFont("media/fonts/arial.ttf", 12, &m_defaultFont);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		pRenderer->BeginScene(true, true, true);

		pRenderer->PushMatrix();
			pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
			pRenderer->SetLookAtCamera(Vector3d(0.0f, 0.0f, 50.0f), Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f));

			glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(50.0f, 50.0f, 0.0);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(100.0f, 50.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(75.0f, 100.0f, 0.0f);
			glEnd();

		pRenderer->PopMatrix();

		pRenderer->PushMatrix();
			pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);

			pRenderer->SetLookAtCamera(Vector3d(0.0f, 0.0f, 50.0f), Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f));

			pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, 10.0f, 0.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "Test Text");
		pRenderer->PopMatrix();

		pRenderer->EndScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}