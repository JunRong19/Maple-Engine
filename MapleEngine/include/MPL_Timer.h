#pragma once

namespace MPL {
	class MPL_Timer {
	public:
		~MPL_Timer();
		MPL_Timer(const MPL_Timer&) = delete;
		MPL_Timer operator=(const MPL_Timer&) = delete;

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
		MPL_Timer();

	private:
		double fps, delta_time;
		// Number of frame since engine starts.
		int frame_count;
	};

	static MPL_Timer& Time = MPL_Timer::Ref();
}




