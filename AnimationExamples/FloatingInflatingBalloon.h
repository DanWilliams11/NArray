#pragma once

#include "InflatingBalloon.h"

namespace ChaoticDynamicalSystems
{
	// classes
	class FloatingInflatingBalloon : public virtual NArray::IAnimatable1D, private InflatingBalloon
	{
		// member functions
	public:
		FloatingInflatingBalloon();
		FloatingInflatingBalloon(const FloatingInflatingBalloon& fib);
		FloatingInflatingBalloon& operator =(const FloatingInflatingBalloon& fib);
		void Copy(const FloatingInflatingBalloon& fib);
		virtual bool GetAscending() const { return InflatingBalloon::GetAscending(); };
		virtual double GetColour() const { return InflatingBalloon::GetColour(); };
		virtual int GetCurrentCount() const { return InflatingBalloon::GetCurrentCount(); };
		virtual double GetData() const { return InflatingBalloon::GetData(); };
		virtual double GetDeltaT() const { return InflatingBalloon::GetDeltaT(); };
		virtual double GetMaxData() const { return InflatingBalloon::GetMaxData(); };
		virtual double GetMaxX() const;
		virtual double GetMinData() const { return InflatingBalloon::GetMinData(); };
		virtual double GetMinX() const;
		virtual double GetScaledData() const { return InflatingBalloon::GetScaledData(); };
		virtual double GetScaledX() const;
		virtual double GetX() const;
		virtual void Update() const { InflatingBalloon::Update(); };
	};
}
