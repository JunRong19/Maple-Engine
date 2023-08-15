#include <pch.h>
#include <MPL_Timer.h>
#include <glhelper.h>

namespace MPL {
	MPL_Timer::MPL_Timer() : frame_count{ 0 }, fps{ 0.0 }, delta_time{ 0.0 } {};
	MPL_Timer::~MPL_Timer() {};

	void MPL_Timer::Initialize() {
		return;
	}

	void MPL_Timer::Tick(double fps_calc_interval) {
		// get elapsed time (in seconds) between previous and current frames
		static double prev_time = glfwGetTime();
		double curr_time = glfwGetTime();
		delta_time = curr_time - prev_time;
		prev_time = curr_time;

		// fps calculations
		static double count = 0.0; // number of game loop iterations
		static double start_time = glfwGetTime();
		// get elapsed time since very beginning (in seconds) ...
		double elapsed_time = curr_time - start_time;

		++count;
		++frame_count;

		// update fps at least every 10 seconds ...
		fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
		fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
		if (elapsed_time > fps_calc_interval) {
			fps = count / elapsed_time;
			start_time = curr_time;
			count = 0.0;
		}
	}
}
