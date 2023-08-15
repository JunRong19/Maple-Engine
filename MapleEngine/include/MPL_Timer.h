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
		void Tick();

		inline float DeltaTime() { return dt; }
		inline float FrameCount() { return frameCount; }

	private:
		MPL_Timer();

	private:
		float dt, lastFrame, frameCount;
	};

	static MPL_Timer& Time = MPL_Timer::Ref();
}




