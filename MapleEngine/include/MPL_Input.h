#pragma once
#include <Event.h>

namespace MPL {
	enum KEY_STATE {
		NONE,
		PRESSED,
		TRIGGERED,
		RELEASED,
		D_CLICK
	};

	/*!***********************************************************************
	\brief
		Contains the events of a key.
	*************************************************************************/
	struct KeyEvent {
		Event<void> onKeyTriggered;
		Event<void> onKeyReleased;
		Event<void> onKeyPressed;
		Event<void> onKeyDoubleClick;
	};

	struct Key {
		int key;
		double clickTime = 0;	// Frame time this key was triggered.
		bool isTriggered = false;
		KeyEvent events;
	};


	class MPL_Input {
	public:
		~MPL_Input();
		MPL_Input(MPL_Input const&) = delete;
		MPL_Input operator=(MPL_Input const&) = delete;

		static MPL_Input& Ref() {
			static MPL_Input ref;
			return ref;
		}

		void Initialize();
		void ProcessInput();
		void Free();

		void SubscribeToKey(int key, KEY_STATE type, void (*func)());
		void UnsubscribeKey(int key, KEY_STATE type, void (*func)());

		inline void SetMousePos(double x, double y) { mousePos.x = x; mousePos.y = y; }

	public:
		Event<double> onMouseScroll;

	private:
		MPL_Input();

	private:
		glm::dvec2 mousePos;

	};
	static MPL_Input& Input = MPL_Input::Ref();
}