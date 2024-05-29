#include "LorenzAttractor.h"

using namespace ChaoticDynamicalSystems;

int LorenzAttractor::m_global_count = 0;

// member functions

LorenzAttractor::LorenzAttractor()
	: m_a(10.0), m_b(7.0), m_c(2.6666), m_delta_t(0.1), m_local_count(m_global_count++)
{
	m_delta_x = 0.0;
	m_delta_y = 0.0;
	m_delta_z = 0.0;
	m_x = 0.1;
	m_y = 0.1;
	m_z = 0.1;
	m_current_count = 0;
}

LorenzAttractor::LorenzAttractor(const LorenzAttractor& la)
	: m_a(la.m_a), m_b(la.m_b), m_c(la.m_c), m_delta_t(la.m_delta_t), m_local_count(m_global_count++)
{
	Copy(la);
}

LorenzAttractor& LorenzAttractor::operator =(const LorenzAttractor& la)
{
	Copy(la);
	return *this;
}

void LorenzAttractor::Copy(const LorenzAttractor& la)
{
	m_delta_x = la.m_delta_x;
	m_delta_y = la.m_delta_y;
	m_delta_z = la.m_delta_z;
	m_x = la.m_x;
	m_y = la.m_y;
	m_z = la.m_z;
	m_current_count = la.m_current_count;
}

double LorenzAttractor::GetColour() const
{
	if (m_local_count < 10)
		return 100.0;

	return 5.0;
}

double LorenzAttractor::GetData() const
{
	return 3.0;
}

double LorenzAttractor::GetScaledData() const
{
	return (GetData() - GetMinData()) / (GetMaxData() - GetMinData());
}

double LorenzAttractor::GetScaledX() const
{
	return (GetX() - GetMinX()) / (GetMaxX() - GetMinX());
}

double LorenzAttractor::GetScaledY() const
{
	return (GetY() - GetMinY()) / (GetMaxY() - GetMinY());
}

double LorenzAttractor::GetScaledZ() const
{
	return (GetZ() - GetMinZ()) / (GetMaxZ() - GetMinZ());
}

double LorenzAttractor::GetX() const
{
	m_delta_x = (m_a * (m_y - m_x)) * m_delta_t;
	return m_x + m_delta_x;
}

double LorenzAttractor::GetY() const
{
	m_delta_y = ((m_b * m_x) - m_y - (m_x * m_z)) * m_delta_t;
	return m_y + m_delta_y;
}

double LorenzAttractor::GetZ() const
{
	m_delta_z = ((m_x * m_y) - (m_c * m_z)) * m_delta_t;
	return m_z + m_delta_z;
}

void LorenzAttractor::Update() const
{
	if (m_current_count >= m_local_count)
	{
		m_x += m_delta_x;
		m_y += m_delta_y;
		m_z += m_delta_z;
	}
	m_current_count++;
}
