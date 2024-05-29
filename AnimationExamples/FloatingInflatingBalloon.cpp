#include "FloatingInflatingBalloon.h"

using namespace ChaoticDynamicalSystems;

// member functions

FloatingInflatingBalloon::FloatingInflatingBalloon()
{
}

FloatingInflatingBalloon::FloatingInflatingBalloon(const FloatingInflatingBalloon& fib)
{
	Copy(fib);
}

FloatingInflatingBalloon& FloatingInflatingBalloon::operator =(const FloatingInflatingBalloon& fib)
{
	Copy(fib);
	return *this;
}

void FloatingInflatingBalloon::Copy(const FloatingInflatingBalloon& fib)
{
	InflatingBalloon::Copy(fib);
}

double FloatingInflatingBalloon::GetMaxX() const
{
	return 10.0;
}

double FloatingInflatingBalloon::GetMinX() const
{
	return 0.0;
}

double FloatingInflatingBalloon::GetScaledX() const
{
	return (GetX() - GetMinX()) / (GetMaxX() - GetMinX());
}

double FloatingInflatingBalloon::GetX() const
{
	return GetCurrentCount() * GetDeltaT();
}
