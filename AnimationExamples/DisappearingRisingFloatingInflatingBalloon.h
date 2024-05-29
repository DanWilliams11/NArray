#pragma once

#include "RisingFloatingInflatingBalloon.h"

namespace ChaoticDynamicalSystems
{
	// classes
	class DisappearingRisingFloatingInflatingBalloon : public virtual NArray::IAnimatable3D, private RisingFloatingInflatingBalloon
	{
		// member functions
	public:
		DisappearingRisingFloatingInflatingBalloon();
		~DisappearingRisingFloatingInflatingBalloon();
		DisappearingRisingFloatingInflatingBalloon(const DisappearingRisingFloatingInflatingBalloon& drfib);
		DisappearingRisingFloatingInflatingBalloon& operator =(const DisappearingRisingFloatingInflatingBalloon& drfib);
		void Copy(const DisappearingRisingFloatingInflatingBalloon& drfib);
		virtual bool GetAscending() const { return RisingFloatingInflatingBalloon::GetAscending(); };
		virtual double GetColour() const { return RisingFloatingInflatingBalloon::GetColour(); };
		virtual int GetCurrentCount() const { return RisingFloatingInflatingBalloon::GetCurrentCount(); };
		virtual double GetData() const { return RisingFloatingInflatingBalloon::GetData(); };
		virtual double GetDeltaT() const { return RisingFloatingInflatingBalloon::GetDeltaT(); };
		virtual double GetMaxData() const { return RisingFloatingInflatingBalloon::GetMaxData(); };
		virtual double GetMaxX() const { return RisingFloatingInflatingBalloon::GetMaxX(); };
		virtual double GetMaxY() const { return RisingFloatingInflatingBalloon::GetMaxY(); };
		virtual double GetMaxZ() const;
		virtual double GetMinData() const { return RisingFloatingInflatingBalloon::GetMinData(); };
		virtual double GetMinX() const { return RisingFloatingInflatingBalloon::GetMinX(); };
		virtual double GetMinY() const { return RisingFloatingInflatingBalloon::GetMinY(); };
		virtual double GetMinZ() const;
		virtual double GetScaledData() const { return RisingFloatingInflatingBalloon::GetScaledData(); };
		virtual double GetScaledX() const { return RisingFloatingInflatingBalloon::GetScaledX(); };
		virtual double GetScaledY() const { return RisingFloatingInflatingBalloon::GetScaledY(); };
		virtual double GetScaledZ() const;
		virtual double GetX() const { return RisingFloatingInflatingBalloon::GetX(); };
		virtual double GetY() const { return RisingFloatingInflatingBalloon::GetY(); };
		virtual double GetZ() const;
		virtual void Update() const { RisingFloatingInflatingBalloon::Update(); };
	};
}
