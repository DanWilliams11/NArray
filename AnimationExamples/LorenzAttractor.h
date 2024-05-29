#pragma once

#include "../Animation/IAnimatable.h"

namespace ChaoticDynamicalSystems
{
	// classes
	class LorenzAttractor : public virtual NArray::IAnimatable3D
	{
		// member data
	private:
		const double m_a;
		const double m_b;
		const double m_c;
		mutable int m_current_count;
		const double m_delta_t;
		mutable double m_delta_x;
		mutable double m_delta_y;
		mutable double m_delta_z;
		static int m_global_count;
		const int m_local_count;
		mutable double m_x;
		mutable double m_y;
		mutable double m_z;

		// member functions
	public:
		LorenzAttractor();
		LorenzAttractor(const LorenzAttractor& la);
		LorenzAttractor& operator =(const LorenzAttractor& la);
		void Copy(const LorenzAttractor& la);
		virtual double GetColour() const;
		virtual double GetData() const;
		virtual double GetMaxData() const { return 1.0; };
		virtual double GetMaxX() const { return 1.0; };
		virtual double GetMaxY() const { return 1.0; };
		virtual double GetMaxZ() const { return 1.0; };
		virtual double GetMinData() const { return 0.0; };
		virtual double GetMinX() const { return -1.0; };
		virtual double GetMinY() const { return -1.0; };
		virtual double GetMinZ() const { return -1.0; };
		virtual double GetScaledData() const;
		virtual double GetScaledX() const;
		virtual double GetScaledY() const;
		virtual double GetScaledZ() const;
		virtual double GetX() const;
		virtual double GetY() const;
		virtual double GetZ() const;
		static void ResetGlobalCount() { m_global_count = 0; };
		virtual void Update() const;
	};
}
