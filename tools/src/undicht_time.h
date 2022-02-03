#ifndef UNDICHT_TIME_H_INCLUDED
#define UNDICHT_TIME_H_INCLUDED


#include <chrono>

namespace undicht {

    namespace tools {

		extern std::chrono::high_resolution_clock::time_point startup_time;
		extern double last_fps_time;


		void initEngineTime();

		double getEngineTime(); // time since initEngineTime()

		double getFPS(); // 1 / (the time since the last call to getFPS())

    } // tools

} // undicht



#endif // UNDICHT_TIME_H_INCLUDED
