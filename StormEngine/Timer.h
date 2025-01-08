/******************************************************************************//**
 * \file			SETimer.h
 * \author 			Khairul	    (100%)
 * \par    			m.binroslan@digipen.edu
 *
 *
 * \brief			Timer Class
 *
 * \date			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
class Timestep
{
public:
	Timestep(float time = 0.0f)
		: m_Time(time)
	{
	}

	operator float() const { return m_Time; }

	float GetSeconds() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }
private:
	float m_Time;
};

//For the time being used as a timer in seconds. Will expand to count minutes and hours.
// some issues when using with splash screen (Due to first frame fps = 0)
class SETimer {
private:
	double m_maxtime{};
	double m_currentTime{};
	double m_totalframes{};
	double m_increment{};
	//double m_curr_fps{};

	bool TimerInit = false;



public:
	SETimer();

	SETimer(double frames);

	bool Countdown(double seconds);

	bool Countup(double seconds);

	double getCurrentTime() { return m_currentTime; }

	double getCurrentPctg() { return (m_currentTime / m_maxtime) ; }

	//void setFPS(double fps) { m_curr_fps = fps; }

	void Reset();

};
