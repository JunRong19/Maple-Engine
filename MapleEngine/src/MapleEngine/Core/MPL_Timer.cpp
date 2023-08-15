#include <pch.h>
#include <MPL_Timer.h>
#include <glhelper.h>

namespace MPL {
	MPL_Timer::MPL_Timer() : dt{ 0.0f }, lastFrame{ 0.0f }, frameCount{ 0.0f } {};
	MPL_Timer::~MPL_Timer() {};

	void MPL_Timer::Initialize() {
		return;
	}

	void MPL_Timer::Tick() {
		GLHelper::update_time(1.0);
	}
}
