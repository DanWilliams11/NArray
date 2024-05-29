#include "RisingFloatingInflatingBalloon.h"

using namespace ChaoticDynamicalSystems;

// member functions

RisingFloatingInflatingBalloon::RisingFloatingInflatingBalloon()
{
}

RisingFloatingInflatingBalloon::~RisingFloatingInflatingBalloon()
{
}

RisingFloatingInflatingBalloon::RisingFloatingInflatingBalloon(const RisingFloatingInflatingBalloon& rfib)
{
	Copy(rfib);
}

RisingFloatingInflatingBalloon& RisingFloatingInflatingBalloon::operator =(const RisingFloatingInflatingBalloon& rfib)
{
	Copy(rfib);
	return *this;
}

void RisingFloatingInflatingBalloon::Copy(const RisingFloatingInflatingBalloon& rfib)
{
	FloatingInflatingBalloon::Copy(rfib);
}

double RisingFloatingInflatingBalloon::GetMaxY() const
{
	return 10.0;
}

double RisingFloatingInflatingBalloon::GetMinY() const
{
	return 0.0;
}

double RisingFloatingInflatingBalloon::GetScaledY() const
{
	return (GetY() - GetMinY()) / (GetMaxY() - GetMinY());
}

double RisingFloatingInflatingBalloon::GetY() const
{
	return GetCurrentCount() * GetDeltaT();
}
