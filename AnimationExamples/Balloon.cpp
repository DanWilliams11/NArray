#include "Balloon.h"

using namespace ChaoticDynamicalSystems;

// member functions

Balloon::Balloon()
	: m_delta_t(0.1)
{
	m_ascending = true;
	m_current_count = 0;
}

Balloon::Balloon(const Balloon& b)
	: m_delta_t(b.m_delta_t)
{
	Copy(b);
}

Balloon& Balloon::operator =(const Balloon& b)
{
	Copy(b);
	return *this;
}

void Balloon::Copy(const Balloon& b)
{
	m_ascending = b.m_ascending;
	m_current_count = b.m_current_count;
}

double Balloon::GetColour() const
{
	return 0.0;
}

void Balloon::Update() const
{
	if (m_current_count == 0)
		m_ascending = true;
	else if (m_current_count == 200)
		m_ascending = false;
	if (m_ascending)
		m_current_count++;
	else
		m_current_count--;
}
