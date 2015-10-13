#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "models/VoxelCharacter.h"
#include "input.h"


extern bool modelWireframe;
extern bool modelTalking;
extern int modelAnimationIndex;
extern VoxelCharacter* pVoxelCharacter;

void KeyPressed(GLFWwindow* window, int key, int scancode, int mods);
void KeyReleased(GLFWwindow* window, int key, int scancode, int mods);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//printf("%08x to %i at %0.3f: Key 0x%04x Scancode 0x%04x (%s) (with%s) was %s\n",
	//	counter++, slot->number, glfwGetTime(), key, scancode,
	//	get_key_name(key),
	//	get_mods_name(mods),
	//	get_action_name(action));

	switch (action)
	{
        case GLFW_PRESS:
		{
			KeyPressed(window, key, scancode, mods);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleased(window, key, scancode, mods);
			break;
		}
		case GLFW_REPEAT:
		{
			break;
		}
	}
}

void KeyPressed(GLFWwindow* window, int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
		{
			modelWireframe = !modelWireframe;
			pVoxelCharacter->SetWireFrameRender(modelWireframe);
			break;
		}
		case GLFW_KEY_E:
		{
			modelTalking = !modelTalking;
			pVoxelCharacter->SetTalkingAnimationEnabled(modelTalking);
			break;
		}
		case GLFW_KEY_Q:
		{
			modelAnimationIndex++;
			if (modelAnimationIndex >= pVoxelCharacter->GetNumAnimations())
			{
				modelAnimationIndex = 0;
			}

			pVoxelCharacter->PlayAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, pVoxelCharacter->GetAnimationName(modelAnimationIndex));
			break;
		}
	}
}

void KeyReleased(GLFWwindow* window, int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
		{
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			break;
		}
		case GLFW_RELEASE:
		{
			break;
		}
		case GLFW_REPEAT:
		{
			break;
		}
	}

	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			break;
		}
		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			break;
		}
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{
			break;
		}
	}
}