#include <stdexcept>

#include "Shape.h"

using namespace NArray;

// member functions

Shape::Shape()
{
	m_order = 0;
	m_shape = nullptr;
}

Shape::Shape(int order, ...)
{
	m_order = order;
	m_shape = new int[order];
	va_list args;
	va_start(args, order);
	for (int i = 1; i <= order; i++)
	{
		int size = va_arg(args, int);
		PutShapeAtCoord(i, size);
	}
	va_end(args);
}

Shape::Shape(const Shape& s)
{
	m_order = s.m_order;
	m_shape = new int[s.m_order];
	for (int i = 0; i < s.m_order; i++)
	{
		m_shape[i] = s.m_shape[i];
	}
}

Shape::~Shape()
{
	DestroyShape();
}

Shape& Shape::operator =(const Shape& s)
{
	if (m_order != s.m_order)
	{
		DestroyShape();
		m_order = s.m_order;
		m_shape = new int[s.m_order];
	}
	for (int i = 0; i < s.m_order; i++)
	{
		m_shape[i] = s.m_shape[i];
	}
	return *this;
}

bool Shape::operator ==(const Shape& s) const
{
	if (m_order != s.m_order)
		return false;
	for (int i = 0; i < m_order; i++)
	{
		if (m_shape[i] != s.m_shape[i])
			return false;
	}
	return true;
}

bool Shape::operator !=(const Shape& s) const
{
	return !operator==(s);
}

void Shape::DestroyShape()
{
	if (m_shape)
	{
		delete[] m_shape;
		m_shape = nullptr;
	}
}

int Shape::GetOrder() const
{
	return m_order;
}

// uses coord to get the size for that coordinate
int Shape::GetShapeAtCoord(int coord) const
{
	int dim = m_order - coord; // the last coordinate is the inner most dimension
	if (dim >= 0 && dim < m_order)
		return m_shape[dim];
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetShapeAtCoord'\n"));
}

// uses dim to get the size for that dimension
int Shape::GetShapeAtDim(int dim) const
{
	if (dim > 0 && dim <= m_order) // zeroth dimension (always size 1) is not represented
		return m_shape[dim - 1]; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetShapeAtDim'\n"));
}

int Shape::GetTotalSize() const
{
	int total_size = 1;
	for (int i = 0; i < m_order; i++)
	{
		total_size *= m_shape[i];
	}
	return total_size;
}

void Shape::Initialise(int size)
{
	for (int i = 0; i < m_order; i++)
	{
		m_shape[i] = size;
	}
}

// uses coord to put the size for that coordinate
void Shape::PutShapeAtCoord(int coord, int size)
{
	int dim = m_order - coord; // the last coordinate is the inner most dimension
	if (dim >= 0 && dim < m_order)
		m_shape[dim] = size;
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutShapeAtCoord'\n"));
}

// uses dim to put the size for that dimension
void Shape::PutShapeAtDim(int dim, int size)
{
	if (dim > 0 && dim <= m_order) // zeroth dimension (always size 1) is not represented
		m_shape[dim - 1] = size; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutShapeAtDim'\n"));
}

void Shape::Read(std::istream& in)
{
	bool fail = false;
	char c;

	if (!fail)
	{
		in >> c;
		if (c != '[')
			fail = true;
	}
	if (!fail)
	{
		for (int i = m_order - 1; i >= 0; i--)
		{
			in >> m_shape[i];
			if (i != 0)
			{
				in >> c;
				if (c != ',')
				{
					fail = true;
					break;
				}
			}
		}
	}
	if (!fail)
	{
		in >> c;
		if (c != ']')
			fail = true;
	}
	if (fail)
		throw(std::out_of_range("NArray: Parse error in 'Shape::Read'\n"));
}

void Shape::ReShape(int order, ...)
{
	if (m_order != order)
	{
		DestroyShape();
		m_order = order;
		m_shape = new int[order];
	}
	va_list args;
	va_start(args, order);
	for (int i = 1; i <= order; i++)
	{
		int size = va_arg(args, int);
		PutShapeAtCoord(i, size);
	}
	va_end(args);
}

Shape Shape::Transpose() const
{
	Shape out(*this);
	for (int i = 0; i < m_order; i++)
	{
		out.m_shape[i] = m_shape[m_order - 1 - i];
	}
	return out;
}

void Shape::Write(std::ostream& out) const
{
	out << '[';
	for (int i = m_order - 1; i >= 0; i--)
	{
		out << m_shape[i];
		if (i != 0)
			out << ", ";
	}
	out << ']' << std::endl;
}

// functions

std::ostream& NArray::operator <<(std::ostream& out, const Shape& s)
{
	s.Write(out);
	return out;
}

std::istream& NArray::operator >>(std::istream& in, Shape& s)
{
	s.Read(in);
	return in;
}
