#include <stdexcept>

#include "Index.h"

using namespace NArray;

// member functions

Index::Index()
{
	m_order = 0;
	m_index = nullptr;
}

Index::Index(int order, ...)
{
	m_order = order;
	m_index = new int[order];
	va_list args;
	va_start(args, order);
	for (int i = 1; i <= order; i++)
	{
		int indice = va_arg(args, int);
		PutIndexAtCoord(i, indice);
	}
	va_end(args);
}

Index::Index(const Index& idx)
{
	m_order = idx.m_order;
	m_index = new int[idx.m_order];
	for (int i = 0; i < idx.m_order; i++)
	{
		m_index[i] = idx.m_index[i];
	}
}

Index::~Index()
{
	DestroyIndex();
}

Index& Index::operator =(const Index& idx)
{
	if (m_order != idx.m_order)
	{
		DestroyIndex();
		m_order = idx.m_order;
		m_index = new int[idx.m_order];
	}
	for (int i = 0; i < idx.m_order; i++)
	{
		m_index[i] = idx.m_index[i];
	}
	return *this;
}

bool Index::operator ==(const Index& idx) const
{
	if (m_order != idx.m_order)
		return false;
	for (int i = 0; i < m_order; i++)
	{
		if (m_index[i] != idx.m_index[i])
			return false;
	}
	return true;
}

bool Index::operator !=(const Index& idx) const
{
	return !operator==(idx);
}

Index Index::operator +(const Index& idx) const
{
	Index out(*this);
	for (int i = 0; i < m_order; i++)
	{
		out.m_index[i] += idx.m_index[i];
	}
	return out;
}

Index Index::operator -(const Index& idx) const
{
	Index out(*this);
	for (int i = 0; i < m_order; i++)
	{
		out.m_index[i] -= idx.m_index[i];
	}
	return out;
}

// calculate the Euclidean distance between two indexes
double Index::CalculateEuclideanDistance(const Index& idx) const
{
	if (m_order != idx.m_order)
		throw(std::invalid_argument("NArray: Indexes must be of the same order in order to calculate the Euclidean distance between them\n"));

	double euclidean_distance = 0.0;
	for (int i = 0; i < m_order; i++)
	{
		euclidean_distance += pow((double) (m_index[i] - idx.m_index[i]), 2);
	}
	euclidean_distance = sqrt(euclidean_distance);
	return euclidean_distance;
}

// calculate the magnitude of the index
double Index::CalculateMagnitude() const
{
	double magnitude = 0.0;
	for (int i = 0; i < m_order; i++)
	{
		magnitude += pow((double) m_index[i], 2);
	}
	magnitude = sqrt(magnitude);
	return magnitude;
}

void Index::DestroyIndex()
{
	if (m_index)
	{
		delete[] m_index;
		m_index = nullptr;
	}
}

// uses coord to get the indice for that coordinate
int Index::GetIndexAtCoord(int coord) const
{
	int dim = m_order - coord; // the last coordinate is the inner most dimension
	if (dim >= 0 && dim < m_order)
		return m_index[dim];
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetIndexAtCoord'\n"));
}

// uses dim to get the indice for that dimension
int Index::GetIndexAtDim(int dim) const
{
	if (dim > 0 && dim <= m_order) // zeroth dimension is not represented
		return m_index[dim - 1]; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'GetIndexAtDim'\n"));
}

int Index::GetOrder() const
{
	return m_order;
}

int* Index::GetPointerToIndex() const
{
	return m_index;
}

void Index::Initialise(int indice)
{
	for (int i = 0; i < m_order; i++)
	{
		m_index[i] = indice - 1; // correct for array access
	}
}

// uses coord to put the indice for that coordinate
void Index::PutIndexAtCoord(int coord, int indice)
{
	int dim = m_order - coord; // the last coordinate is the inner most dimension
	if (dim >= 0 && dim < m_order)
		m_index[dim] = indice - 1; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutIndexAtCoord'\n"));
}

// uses dim to put the indice for that dimension
void Index::PutIndexAtDim(int dim, int indice)
{
	if (dim > 0 && dim <= m_order) // zeroth dimension is not represented
		m_index[dim - 1] = indice; // correct for array access
	else
		throw(std::out_of_range("NArray: Dimension out of bounds in 'PutIndexAtDim'\n"));
}

void Index::ReIndex(int order, ...)
{
	if (m_order != order)
	{
		DestroyIndex();
		m_order = order;
		m_index = new int[order];
	}
	va_list args;
	va_start(args, order);
	for (int i = 1; i <= order; i++)
	{
		int indice = va_arg(args, int);
		PutIndexAtCoord(i, indice);
	}
	va_end(args);
}

Index Index::Transpose() const
{
	Index out(*this);
	for (int i = 0; i < m_order; i++)
	{
		out.m_index[i] = m_index[m_order - 1 - i];
	}
	return out;
}

void Index::Write(std::ostream& out) const
{
	out << '[';
	for (int i = m_order - 1; i >= 0; i--)
	{
		out << m_index[i];
		if (i != 0)
			out << ", ";
	}
	out << ']';
}

// functions

std::ostream& NArray::operator <<(std::ostream& out, const Index& idx)
{
	idx.Write(out);
	return out;
}
