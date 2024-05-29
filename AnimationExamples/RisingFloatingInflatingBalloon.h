#pragma once

#include "FloatingInflatingBalloon.h"

namespace ChaoticDynamicalSystems
{
	// classes
	class RisingFloatingInflatingBalloon : public virtual NArray::IAnimatable2D, private FloatingInflatingBalloon
	{
		// member functions
	public:
		RisingFloatingInflatingBalloon();
		~RisingFloatingInflatingBalloon();
		RisingFloatingInflatingBalloon(const RisingFloatingInflatingBalloon& rfib);
		RisingFloatingInflatingBalloon& operator =(const RisingFloatingInflatingBalloon& rfib);
		void Copy(const RisingFloatingInflatingBalloon& rfib);
		virtual bool GetAscending() const { return FloatingInflatingBalloon::GetAscending(); };
		virtual double GetColour() const { return FloatingInflatingBalloon::GetColour(); };
		virtual int GetCurrentCount() const { return FloatingInflatingBalloon::GetCurrentCount(); };
		virtual double GetData() const { return FloatingInflatingBalloon::GetData(); };
		virtual double GetDeltaT() const { return FloatingInflatingBalloon::GetDeltaT(); };
		virtual double GetMaxData() const { return FloatingInflatingBalloon::GetMaxData(); };
		virtual double GetMaxX() const { return FloatingInflatingBalloon::GetMaxX(); };
		virtual double GetMaxY() const;
		virtual double GetMinData() const { return FloatingInflatingBalloon::GetMinData(); };
		virtual double GetMinX() const { return FloatingInflatingBalloon::GetMinX(); };
		virtual double GetMinY() const;
		virtual double GetScaledData() const { return FloatingInflatingBalloon::GetScaledData(); };
		virtual double GetScaledX() const { return FloatingInflatingBalloon::GetScaledX(); };
		virtual double GetScaledY() const;
		virtual double GetX() const { return FloatingInflatingBalloon::GetX(); };
		virtual double GetY() const;
		virtual void Update() const { FloatingInflatingBalloon::Update(); };
	};
}
