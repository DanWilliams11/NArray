#include <stdexcept>

#include "Windows.h"

#include "Timer.h"

namespace NArray
{
	Timer::Timer()
	{
		m_ctr1 = 0;
		m_ctr2 = 0;
		m_freq = 0;
		m_started = false;
		m_time = 0.0;
		m_total_time = 0.0;
	}

	double Timer::GetTime()
	{
		return m_time;
	}

	double Timer::GetTotalTime()
	{
		return m_total_time;
	}

	void Timer::StartTimer()
	{
		if (m_started)
			throw(std::runtime_error("NArray: Failed to start timer - the timer is already running\n"));

		if (!QueryPerformanceCounter((LARGE_INTEGER*) &m_ctr1))
			throw(std::runtime_error("NArray: Failed to start timer - could not query counter\n"));

		m_started = true;
	}

	void Timer::StopTimer()
	{
		if (!m_started)
			throw(std::runtime_error("NArray: Failed to stop timer - the timer is not running\n"));

		if (!QueryPerformanceCounter((LARGE_INTEGER*) &m_ctr2))
			throw(std::runtime_error("NArray: Failed to stop timer - could not query counter\n"));

		if (!QueryPerformanceFrequency((LARGE_INTEGER*) &m_freq))
			throw(std::runtime_error("NArray: Failed to stop timer - could not query frequency\n"));

		m_time = (m_ctr2 - m_ctr1) * 1.0 / m_freq;
		m_total_time += m_time;
		m_started = false;
	}
}
