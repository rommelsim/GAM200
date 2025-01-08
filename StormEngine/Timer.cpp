/******************************************************************************//**
 * \file			SETimer.cpp
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
#include "pch.h"
#include "SEGraphics.h"
#include "Timer.h"

SETimer::SETimer()
{
	m_currentTime = 0;
	m_totalframes = 0;
	//m_curr_fps = 0;
	m_increment = 0;
	m_maxtime = 0;
	TimerInit = true;
}

SETimer::SETimer(double frames) {
	(void)frames;
	m_currentTime = 0;
	//m_curr_fps = frames;
	m_increment = 0;
	m_maxtime = 0;
	TimerInit = true;
}

//Use in if statement
//returns false if timer is not done counting
//returns true if timer is done
bool SETimer::Countdown(double seconds)
{
	if (TimerInit) {
		m_currentTime = m_maxtime = seconds;
		if (m_totalframes == 0) {
			m_totalframes = seconds * SEGetFPS();
		}

		else {
			m_totalframes = seconds * m_totalframes;
		}

		m_increment = m_maxtime / m_totalframes;
		TimerInit = false;
	}

	if (m_currentTime <= 0) {
		return true;
	}

	m_currentTime -= m_increment;
	return false;
}

//Use in if statement
//returns false if timer is not done counting
//returns true if timer is done
bool SETimer::Countup(double seconds)
{
	if (TimerInit) {
		m_maxtime = seconds;
		if (m_totalframes == 0) {
			m_totalframes = seconds * SEGetFPS();
		}

		else {
			m_totalframes = seconds * m_totalframes;
		}
		m_increment = m_maxtime / m_totalframes;
		TimerInit = false;
	}

	if (m_currentTime >= m_maxtime) {
		return true;
	}

	m_currentTime += m_increment;
	return false;
}

void SETimer::Reset()
{
	m_currentTime = 0;
	m_totalframes = 0;
	m_increment = 0;
	m_maxtime = 0;
	TimerInit = true;
}
