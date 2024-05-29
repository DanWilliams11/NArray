#include <stdexcept>

#include "Mask.h"

using namespace NArray;

// member functions

Mask::Mask()
{
	m_order = 0;
	m_mask = nullptr;
	m_zeroth_dimension = not_use;
}

Mask::Mask(int order, ...)
{
	m_order = order;
	m_mask = new int[order];
	va_list args;
	va_start(args, order);
	for (int i = 1; i <= order; i++)
	{
		int specifier = va_arg(args, int);
		PutMaskAtCoord(i, specifier);
	}
	va_end(args);
	m_zeroth_dimension = not_use;
}

Mask::Mask(const Mask& m)
{
	m_order = m.m_order;
	m_mask = new int[m.m_order];
	for (int i = 0; i < m.m_order; i++)
	{
		m_mask[i] = m.m_mask[i];
	}
	m_zeroth_dimension = m.m_zeroth_dimension;
}

Mask::~Mask()
{
	DestroyMask();
}

Mask& Mask::operator =(const Mask& m)
{
	if (m_order != m.m_order)
	{
		DestroyMask();
		m_order = m.m_order;
		m_mask = new int[m.m_order];
	}
	for (int i = 0; i < m.m_order; i++)
	{
		m_mask[i] = m.m_mask[i];
	}
	m_zeroth_dimension = m.m_zeroth_dimension;
	return *this;
}

bool Mask::operator ==(const Mask& m) const
{
	if (m_order != m.m_order)
		return false;
	for (int i = 0; i < m_order; i++)
	{
		if (m_mask[i] != m.m_mask[i])
			return false;
	}
	if (m_zeroth_dimension != m.m_zeroth_dimension)
		return false;
	return true;
}

bool Mask::operator !=(const Mask& m) const
{
	return !operator==(m);
}

void Mask::DestroyMask()
{
	if (m_mask)
	{
		delete[] m_mask;
		m_mask = nullptr;
	}
}

// uses coord to get the specifier for that coordinate
int Mask::GetMaskAtCoord(int coord) const
{
	int dim = m_order - coord; // the last coordinate is the inner most dimension
	if (dim >= 0 && dim < m_order)
		return m_mask[dim];
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetMaskAtCoord'\n"));
}

// uses dim to get the specifier for that dimension
int Mask::GetMaskAtDim(int dim) const
{
	if (dim == 0) // special case for multi-threading masks
		return m_zeroth_dimension;
	else if (dim > 0 && dim <= m_order) // zeroth dimension is not represented
		return m_mask[dim - 1]; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetMaskAtDim'\n"));
}

int Mask::GetOrder() const
{
	return m_order;
}

void Mask::Initialise(int specifier)
{
	if (specifier > 0)
		specifier -= 1; // correct for array access
	for (int i = 0; i < m_order; i++)
	{
		m_mask[i] = specifier;
	}
}

// uses coord to put the specifier for that coordinate
void Mask::PutMaskAtCoord(int coord, int specifier)
{
	int dim = m_order - coord; // the last coordinate is the inner most dimension
	if (specifier > 0)
		specifier -= 1; // correct for array access
	if (dim >= 0 && dim < m_order)
		m_mask[dim] = specifier;
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutMaskAtCoord'\n"));
}

// uses dim to put the specifier for that dimension
void Mask::PutMaskAtDim(int dim, int specifier)
{
	if (dim == 0) // special case for multi-threading masks
		m_zeroth_dimension = specifier;
	else if (dim > 0 && dim <= m_order)
		m_mask[dim - 1] = specifier; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutMaskAtDim'\n"));
}

void Mask::ReMask(int order, ...)
{
	if (m_order != order)
	{
		DestroyMask();
		m_order = order;
		m_mask = new int[order];
	}
	va_list args;
	va_start(args, order);
	for (int i = 1; i <= order; i++)
	{
		int specifier = va_arg(args, int);
		PutMaskAtCoord(i, specifier);
	}
	va_end(args);
}

Mask Mask::Transpose() const
{
	Mask out(*this);
	for (int i = 0; i < m_order; i++)
	{
		out.m_mask[i] = m_mask[m_order - 1 - i];
	}
	return out;
}

void Mask::Write(std::ostream& out) const
{
	out << '[';
	for (int i = m_order - 1; i >= 0; i--)
	{
		out << m_mask[i];
		if (i != 0)
			out << ", ";
	}
	out << ']';
}

// functions

std::ostream& NArray::operator <<(std::ostream& out, const Mask& m)
{
	m.Write(out);
	return out;
}
