#pragma once

#include <cstdarg>
#include <ostream>

namespace NArray
{
	// classes
	// class for specifying a mask onto an n-dimensional array (specifier for each dimension)
	class Mask
	{
		// member variables
	public:
		static 	const int not_use = 0;
		static	const int use_all = -1;
	private:
				int* m_mask;
				int m_order;
				int m_zeroth_dimension;
		// member functions
	private:
				void DestroyMask();
	public:
				Mask();
				Mask(int order, ...);
				Mask(const Mask& m);
		virtual ~Mask();
				Mask& operator =(const Mask& m);
				bool operator ==(const Mask& m) const;
				bool operator !=(const Mask& m) const;
				int GetMaskAtCoord(int coord) const;
				int GetMaskAtDim(int dim) const;
				int GetOrder() const;
				void Initialise(int specifier = use_all);
				void PutMaskAtCoord(int coord, int specifier);
				void PutMaskAtDim(int dim, int specifier);
				void ReMask(int order, ...);
				Mask Transpose() const;
				void Write(std::ostream& out) const;
	};

	// functions
	std::ostream& operator <<(std::ostream& out, const Mask& m);
}
