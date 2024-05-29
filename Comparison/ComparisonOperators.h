#pragma once

#include "../Core/Operator.h"
#include "IComparable.h"

namespace NArray
{
	// class for comparing the corresponding values in two n-dimensional arrays using a fixed comparator - tested
	// this operator cannot be multi-threaded because it's return value is needed for further computation
	// the results of each comparisson need to be ANDed to produce an overall result
	template <class S, class V> class Compare : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		const IComparable<V>& m_comparator;
		// member functions
	private:
		Compare(const Compare&);
		Compare& operator =(const Compare&);
	public:
		Compare(const IComparable<V>& comparator)
			: m_comparator(comparator)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in1, S& in2)
		{
			in1; // do this to prevent compiler warning 'unreferenced formal parameter'
			return m_comparator.ApplyComparator(value, in2.GetValue(index));
		}
	};
}
