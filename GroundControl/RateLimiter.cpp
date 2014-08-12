#include "stdafx.h"
#include "RateLimiter.h"




RateLimiter::RateLimiter(float rateLimit, float initVal) : m_curVal(initVal), m_rateLimit(rateLimit)
{
}

RateLimiter::RateLimiter(float rateLimit, const Eigen::Vector2f& initVal) : m_curVec(initVal), m_rateLimit(rateLimit)
{
}



RateLimiter::~RateLimiter()
{
}

float RateLimiter::operator()(float desired)
{
	if (fabs(desired - m_curVal) > m_rateLimit)
	{
		if (desired > m_curVal)
		{
			m_curVal += m_rateLimit;
		}
		else
		{
			m_curVal -= m_rateLimit;
		}
	}
	else
	{
		m_curVal = desired;
	}

	return m_curVal;
}

Eigen::Vector2f RateLimiter::operator()(const Eigen::Vector2f& desired)
{
	Eigen::Vector2f diff = desired - m_curVec;
	
	if (diff.norm() > m_rateLimit)
	{
		diff.normalize();
		m_curVec = m_curVec + diff * m_rateLimit;
	}
	else
	{
		m_curVec = desired;
	}

	return m_curVec;



}

