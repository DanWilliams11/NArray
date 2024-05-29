#pragma once

#include <cstdarg>
#include <istream>
#include <ostream>

namespace NArray
{
	// classes
	// class for specifying the shape of an n-dimensional array (size for each dimension)
	class Shape
	{
		// member variables
	private:
				int m_order;
				int* m_shape;
		// member functions
	private:
				void DestroyShape();
	public:
				Shape();
				Shape(int order, ...);
				Shape(const Shape& s);
		virtual ~Shape();
				Shape& operator =(const Shape& s);
				bool operator ==(const Shape& s) const;
				bool operator !=(const Shape& s) const;
				int GetOrder() const;
				int GetShapeAtCoord(int coord) const;
				int GetShapeAtDim(int dim) const;
				int GetTotalSize() const;
				void Initialise(int size = 1);
				void PutShapeAtCoord(int coord, int size);
				void PutShapeAtDim(int dim, int size);
				void Read(std::istream& in);
				void ReShape(int order, ...);
				Shape Transpose() const;
				void Write(std::ostream& out) const;
	};

	// functions
	std::ostream& operator <<(std::ostream& out, const Shape& s);
	std::istream& operator >>(std::istream& in, Shape& s);
}
