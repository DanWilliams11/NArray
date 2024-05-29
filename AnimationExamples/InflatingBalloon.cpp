#include "InflatingBalloon.h"

using namespace ChaoticDynamicalSystems;

// member functions

InflatingBalloon::InflatingBalloon()
{
}

InflatingBalloon::InflatingBalloon(const InflatingBalloon& ib)
{
	Copy(ib);
}

InflatingBalloon& InflatingBalloon::operator =(const InflatingBalloon& ib)
{
	Copy(ib);
	return *this;
}

void InflatingBalloon::Copy(const InflatingBalloon& ib)
{
	Balloon::Copy(ib);
}

double InflatingBalloon::GetData() const
{
	return GetCurrentCount() * GetDeltaT();
}

double InflatingBalloon::GetMaxData() const
{
	return 10.0;
}

double InflatingBalloon::GetMinData() const
{
	return 0.0;
}

double InflatingBalloon::GetScaledData() const
{
	return ((GetData() - GetMinData()) / (GetMaxData() - GetMinData())) * 15.0;
}
