#pragma once

#include "Balloon.h"

namespace ChaoticDynamicalSystems
{
	// classes
	class InflatingBalloon : public virtual NArray::IAnimatable0D, private Balloon
	{
		// member functions
	public:
		InflatingBalloon();
		InflatingBalloon(const InflatingBalloon& ib);
		InflatingBalloon& operator =(const InflatingBalloon& ib);
		void Copy(const InflatingBalloon& ib);
		virtual bool GetAscending() const { return Balloon::GetAscending(); };
		virtual double GetColour() const { return Balloon::GetColour(); };
		virtual int GetCurrentCount() const { return Balloon::GetCurrentCount(); };
		virtual double GetData() const;
		virtual double GetDeltaT() const { return Balloon::GetDeltaT(); };
		virtual double GetMaxData() const;
		virtual double GetMinData() const;
		virtual double GetScaledData() const;
		virtual void Update() const { Balloon::Update(); };
	};
}
