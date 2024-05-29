#pragma once

namespace NArray
{
	class Timer
	{
		// member variables
	private:
		__int64 m_ctr1;
		__int64 m_ctr2;
		__int64 m_freq;
		bool m_started;
		double m_time;
		double m_total_time;
		// member functions
	public:
		Timer();
		double GetTime();
		double GetTotalTime();
		void StartTimer();
		void StopTimer();
	};
}
