#include "DisappearingRisingFloatingInflatingBalloon.h"

using namespace ChaoticDynamicalSystems;

// member functions

DisappearingRisingFloatingInflatingBalloon::DisappearingRisingFloatingInflatingBalloon()
{
}

DisappearingRisingFloatingInflatingBalloon::~DisappearingRisingFloatingInflatingBalloon()
{
}

DisappearingRisingFloatingInflatingBalloon::DisappearingRisingFloatingInflatingBalloon(const DisappearingRisingFloatingInflatingBalloon& drfib)
{
	Copy(drfib);
}

DisappearingRisingFloatingInflatingBalloon& DisappearingRisingFloatingInflatingBalloon::operator =(const DisappearingRisingFloatingInflatingBalloon& drfib)
{
	Copy(drfib);
	return *this;
}

void DisappearingRisingFloatingInflatingBalloon::Copy(const DisappearingRisingFloatingInflatingBalloon& drfib)
{
	RisingFloatingInflatingBalloon::Copy(drfib);
}

double DisappearingRisingFloatingInflatingBalloon::GetMaxZ() const
{
	return 10.0;
}

double DisappearingRisingFloatingInflatingBalloon::GetMinZ() const
{
	return 0.0;
}

double DisappearingRisingFloatingInflatingBalloon::GetScaledZ() const
{
	return (GetZ() - GetMinZ()) / (GetMaxZ() - GetMinZ());
}

double DisappearingRisingFloatingInflatingBalloon::GetZ() const
{
	return GetCurrentCount() * GetDeltaT();
}
