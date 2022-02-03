#include "undicht_time.h"

using namespace std::chrono;

namespace undicht {

    namespace tools {

		

		std::chrono::high_resolution_clock::time_point startup_time;
		double last_fps_time;


		void initEngineTime() {

			startup_time = high_resolution_clock::now();

		}

		double getEngineTime() {
			// time since initEngineTime()

			high_resolution_clock::time_point now = high_resolution_clock::now();

			duration<double> time_since_startup = duration_cast<duration<double>>(now - startup_time);

			return time_since_startup.count();
		}

		double getFPS() {
			// 1 / (the time since the last call to getFPS())

			double fps = 1 / (getEngineTime() - last_fps_time);

			last_fps_time = getEngineTime();

			return fps;
		}

    } // tools

} // undicht
