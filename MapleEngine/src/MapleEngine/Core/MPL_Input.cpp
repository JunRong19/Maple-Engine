#include <pch.h>
#include <MPL_Input.h>
#include <MPL_Engine.h>
#include <MPL_Timer.h>

namespace MPL {

	void keyCallback(GLFWwindow* window, int key, int action);
	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseBtnCallback(GLFWwindow* window, int buttton, int action, int mods);

	void mousePosCallback(GLFWwindow* window, double xPos, double yPos);
	void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	const float DOUBLE_CLICK_TIME = 0.2f;	// Amount of seconds between clicks triggered to be counted as double clicking.

	// Store keys to use.
	std::vector<Key> keys;

	MPL_Input::MPL_Input() : mousePos{} {
		return;
	}

	MPL_Input::~MPL_Input() {
		return;
	}

	void MPL_Input::Initialize() {
		glfwSetKeyCallback(Core.Window(), keyboardCallback);
		glfwSetMouseButtonCallback(Core.Window(), mouseBtnCallback);


		glfwSetCursorPosCallback(Core.Window(), mousePosCallback);
		glfwSetScrollCallback(Core.Window(), mouseScrollCallback);
	}

	void MPL_Input::ProcessInput() {
		if (glfwGetKey(Core.Window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(Core.Window(), true);

	}

	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		keyCallback(window, key, action);
	}

	void mouseBtnCallback(GLFWwindow* window, int buttton, int action, int mods) {
		keyCallback(window, buttton, action);
	}

	void keyCallback(GLFWwindow* window, int key, int action) {
		for (auto& k : keys) {
			if (key == k.key) {
				if (glfwGetKey(window, key) == GLFW_PRESS || glfwGetMouseButton(window, key) == GLFW_PRESS) {
					if (!k.isTriggered) {

						// Key double click.
						if (glfwGetTime() - k.clickTime < DOUBLE_CLICK_TIME) {
							k.events.onKeyDoubleClick.Invoke();
							std::cout << "d click" << std::endl;
						}
						else {
							// Key triggered.
							k.isTriggered = true;
							k.events.onKeyTriggered.Invoke();
							std::cout << "triggered" << std::endl;
						}

						k.clickTime = glfwGetTime();
					}
					else {
						// Key pressed.
						k.events.onKeyPressed.Invoke();
						std::cout << "pressed" << std::endl;
					}
				}
				else {
					// Key released.
					k.isTriggered = false;
					k.events.onKeyReleased.Invoke();
					std::cout << "released" << std::endl;
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
		Subscribe func to key event.
	\param key
		Which key.
	\param type
		Type of key event to call function.
	\param func
		Callback function when key event is invoke.
	*************************************************************************/
	void MPL_Input::SubscribeToKey(int key, KEY_STATE type, void (*func)()) {
		for (auto& k : keys) {
			// Subscribe to key event if key is found.
			if (k.key == key) {
				switch (type) {
				case TRIGGERED:
					k.events.onKeyTriggered.Subscribe(func);
					return;
				case RELEASED:
					k.events.onKeyReleased.Subscribe(func);
					return;
				case PRESSED:
					k.events.onKeyPressed.Subscribe(func);
					return;
				case D_CLICK:
					k.events.onKeyDoubleClick.Subscribe(func);
					return;
				default:
					std::cout << "ERROR: Unknown KEY_STATE";
					return;
				}
			}
		}

		// Create new key if key is not found.
		Key newKey{};
		newKey.key = key;

		// Subscribe to key event.
		switch (type) {

		case TRIGGERED:
			newKey.events.onKeyTriggered.Subscribe(func);
			break;
		case RELEASED:
			newKey.events.onKeyReleased.Subscribe(func);
			break;
		case PRESSED:
			newKey.events.onKeyPressed.Subscribe(func);
			break;
		case D_CLICK:
			newKey.events.onKeyDoubleClick.Subscribe(func);
			break;
		default:
			std::cout << "ERROR: Unknown KEY_STATE";
			break;
		}
		// Add new key to deque.
		keys.push_back(newKey);
	}

	/*!***********************************************************************
	\brief
		Unsubscribe func from key event.
	\param key
		Which key.
	\param type
		Type of key event to unsubscribe function.
	\param func
		Function to unsubscribe.
	*************************************************************************/
	void MPL_Input::UnsubscribeKey(int key, KEY_STATE type, void (*func)()) {
		for (auto& k : keys) {
			if (k.key == key) {
				switch (type) {
				case TRIGGERED:
					k.events.onKeyTriggered.Unsubscribe(func);
					return;
				case RELEASED:
					k.events.onKeyReleased.Unsubscribe(func);
					return;
				case PRESSED:
					k.events.onKeyPressed.Unsubscribe(func);
					return;
				case D_CLICK:
					k.events.onKeyDoubleClick.Unsubscribe(func);
					return;
				default:
					std::cout << "ERROR: Unknown KEY_STATE";
					return;
				}
			}
		}
	}

	void mousePosCallback(GLFWwindow* window, double xPos, double yPos) {
		Input.SetMousePos(xPos, yPos);
	}

	void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
		Input.onMouseScroll.Invoke(yOffset);
	}

	void MPL_Input::Free() {
		// Free all key events.
		for (auto& k : keys) {
			k.events.onKeyTriggered.UnsubscribeAll();
			k.events.onKeyPressed.UnsubscribeAll();
			k.events.onKeyReleased.UnsubscribeAll();
			k.events.onKeyDoubleClick.UnsubscribeAll();
		}
	}
}
