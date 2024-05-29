#pragma once

#include <cstdarg>

#include "../Core/Operator.h"

namespace NArray
{
	// classes
	// class for specifying a function composition of operators to be applied in turn to the values in an n-dimensional array
	template <class S, class V> class Composition : public MultiThreadedOperator<S, V>
	{
		// member variables
	private:
				SingleThreadedOperator<S, V>** m_composition;
				int m_size;
		// member functions
	private:
				void DestroyConposition();
	public:
				Composition();
				Composition(int size, ...);
				Composition(const Composition& c);
				virtual ~Composition();
				Composition& operator =(const Composition& c);
				virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out);
				SingleThreadedOperator<S, V>* GetCompositionAtPosition(int position) const;
				int GetSize() const;
				void PutCompositionAtReversePosition(int position, SingleThreadedOperator<S, V>* op);
				void ReComposition(int size, ...);
	};
}
