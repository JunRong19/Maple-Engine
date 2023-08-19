#pragma once

namespace MPL {
	class MPL_Timer {
	public:
		static MPL_Timer& Ref() {
			static MPL_Timer ref;
			return ref;
		}

		void Initialize();
		void Tick(double fps_calc_interval = 1.0);

		inline double Fps() { return fps; }
		inline double DeltaTime() { return delta_time; }
		inline int FrameCount() { return frame_count; }

	private:
		~MPL_Timer();
		MPL_Timer(const MPL_Timer&) = delete;
		MPL_Timer operator=(const MPL_Timer&) = delete;

	private:
		MPL_Timer();

	private:
		// Number of frame since engine starts.
		int frame_count;
		double fps, delta_time;
	};

	static MPL_Timer& Time = MPL_Timer::Ref();
}




