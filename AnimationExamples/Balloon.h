#pragma once

#include "../Animation/IAnimatable.h"

namespace ChaoticDynamicalSystems
{
	// classes
	class Balloon : public virtual NArray::IAnimatable
	{
		// member data
	private:
		mutable bool m_ascending;
		mutable int m_current_count;
		const double m_delta_t;

		// member functions
	public:
		Balloon();
		Balloon(const Balloon& b);
		Balloon& operator =(const Balloon& b);
		void Copy(const Balloon& b);
		virtual bool GetAscending() const { return m_ascending; };
		virtual double GetColour() const;
		virtual int GetCurrentCount() const { return m_current_count; };
		virtual double GetDeltaT() const { return m_delta_t; };
		virtual void Update() const;
	};
}
