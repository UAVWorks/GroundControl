#pragma once

#include <Eigen/Core>

class RateLimiter
{
public:
	RateLimiter(float rateLimit, float initVal);
	RateLimiter(float rateLimit, const Eigen::Vector2f& initVal);
	~RateLimiter();



	float operator()(float desired);
	Eigen::Vector2f operator()(const Eigen::Vector2f& desired);

private:
	float m_curVal;
	Eigen::Vector2f m_curVec;
	float m_rateLimit;
};

