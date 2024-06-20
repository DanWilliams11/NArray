#pragma once

#include "Index.h"
#include "Mask.h"

namespace NArray
{
	// classes
	template <class S, class V> class SingleThreadedOperator abstract
	{
		// member functions
	public:
		virtual ~SingleThreadedOperator() {};
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out) = 0;
	};

	template <class S, class V> class MultiThreadedOperator abstract : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		Mask m_mask;
		bool m_use_mask;
		// member functions
	public:
		MultiThreadedOperator()
		{
			m_use_mask = false;
		}
		virtual ~MultiThreadedOperator() {};
		void AddMask(Mask& mask)
		{
			m_mask = mask;
			m_use_mask = true;
		}
		int GetMaskAtDim(int dim) const
		{
			return m_mask.GetMaskAtDim(dim);
		}
		void UseMask(bool use_mask)
		{
			m_use_mask = use_mask;
		}
		bool UsingMask() const
		{
			return m_use_mask;
		}
	};
}
