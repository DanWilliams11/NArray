#pragma once

#include "../Core/Operator.h"
#include "ICombinable.h"

namespace NArray
{
	// class for combining the corresponding values in two n-dimensional arrays using a fixed combiner, writing the results to a fixed third n-dimensional array - tested
	template <class S, class V> class Combine : public MultiThreadedOperator<S, V>
	{
		// member variables
	private:
		const ICombinable<V>& m_combiner;
		S& m_out;
		// member functions
	private:
		Combine(const Combine&);
		Combine& operator =(const Combine&);
	public:
		Combine(const ICombinable<V>& combiner, S& out)
			: m_combiner(combiner), m_out(out)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in1, S& in2)
		{
			in1; // do this to prevent compiler warning 'unreferenced formal parameter'
			m_out.PutValue(index, m_combiner.ApplyCombiner(value, in2.GetValue(index)));
			return true;
		}
	};
}
