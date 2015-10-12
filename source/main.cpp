#include "Renderer/Renderer.h"
#include "Renderer/camera.h"
#include "utils/Interpolator.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/* Create a windowed mode window and it's OpenGL context */
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
	glfwSwapInterval(0); // Disable v-sync

	/* Create the renderer */
	Renderer* pRenderer = new Renderer(windowWidth, windowHeight, 32, 8);

	/* Create cameras */
	Camera* pGameCamera = new Camera(pRenderer);
	pGameCamera->SetPosition(Vector3d(0.0f, 0.0f, 3.0f));
	pGameCamera->SetFacing(Vector3d(0.0f, 0.0f, -1.0f));
	pGameCamera->SetUp(Vector3d(0.0f, 1.0f, 0.0f));
	pGameCamera->SetRight(Vector3d(1.0f, 0.0f, 0.0f));

	/* Create viewports */
	unsigned int defaultViewport;
	pRenderer->CreateViewport(0, 0, windowWidth, windowHeight, 60.0f, &defaultViewport);

	/* Create fonts */
	unsigned int defaultFont;
	pRenderer->CreateFreeTypeFont("media/fonts/arial.ttf", 12, &defaultFont);

	/* Setup the FPS counters */
	LARGE_INTEGER fps_previousTicks;
	LARGE_INTEGER fps_ticksPerSecond;
	QueryPerformanceCounter(&fps_previousTicks);
	QueryPerformanceFrequency(&fps_ticksPerSecond);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Delta time
		double timeNow = (double)timeGetTime() / 1000.0;
		static double timeOld = timeNow - (1.0 / 50.0);
		float deltaTime = (float)timeNow - (float)timeOld;
		timeOld = timeNow;

		// FPS
		LARGE_INTEGER fps_currentTicks;
		QueryPerformanceCounter(&fps_currentTicks);
		float fps = 1.0f / ((float)(fps_currentTicks.QuadPart - fps_previousTicks.QuadPart) / (float)fps_ticksPerSecond.QuadPart);
		fps_previousTicks = fps_currentTicks;

		// Update interpolator singleton
		Interpolator::GetInstance()->Update();

		// Begin rendering
		pRenderer->BeginScene(true, true, true);

		pRenderer->PushMatrix();
			// Set the default projection mode
			pRenderer->SetProjectionMode(PM_PERSPECTIVE, defaultViewport);

			// Set the lookat camera
			pGameCamera->Look();

			glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(-1.0f, -1.0f, 0.0);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(1.0f, -1.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(0.0f, 0.75f, 0.0f);
			glEnd();

		pRenderer->PopMatrix();

		char lFPSBuff[128];
		sprintf_s(lFPSBuff, "FPS: %.0f  Delta: %.4f", fps, deltaTime);

		pRenderer->PushMatrix();
			pRenderer->SetProjectionMode(PM_2D, defaultViewport);
			pRenderer->SetLookAtCamera(Vector3d(0.0f, 0.0f, 50.0f), Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f));

			pRenderer->RenderFreeTypeText(defaultFont, 10.0f, 10.0f, 0.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lFPSBuff);
		pRenderer->PopMatrix();

		// End rendering
		pRenderer->EndScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}