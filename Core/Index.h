#pragma once

#include <cstdarg>
#include <ostream>

namespace NArray
{
	// classes
	// class for specifying an index into an n-dimensional array (indice for each dimension)
	class Index
	{
		// member variables
	private:
				int* m_index;
				int m_order;
		// member functions
	private:
				void DestroyIndex();
	public:
				Index();
				Index(int order, ...);
				Index(const Index& idx);
		virtual	~Index();
				Index& operator =(const Index& idx);
				bool operator ==(const Index& idx) const;
				bool operator !=(const Index& idx) const;
				Index operator +(const Index& idx) const;
				Index operator -(const Index& idx) const;
				double CalculateEuclideanDistance(const Index& index) const;
				double CalculateMagnitude() const;
				int GetIndexAtCoord(int coord) const;
				int GetIndexAtDim(int dim) const;
				int GetOrder() const;
				int* GetPointerToIndex() const;
				void Initialise(int indice = 1);
				void PutIndexAtCoord(int coord, int indice);
				void PutIndexAtDim(int dim, int indice);
				void ReIndex(int order, ...);
				Index Transpose() const;
				void Write(std::ostream& out) const;
	};

	// functions
	std::ostream& operator <<(std::ostream& out, const Index& idx);
}
