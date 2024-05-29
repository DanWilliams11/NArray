#pragma once

#include "Index.h"
#include "Mask.h"

namespace NArray
{
	// classes
	template <class S1, class V1, class S2, class V2> class SingleThreadedTransform abstract
	{
		// member functions
	public:
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S2& out) = 0;
	};

	template <class S1, class V1, class S2, class V2> class MultiThreadedTransform abstract : public SingleThreadedTransform<S1, V1, S2, V2>
	{
		// member variables
	private:
		Mask m_mask;
		bool m_use_mask;
		// member functions
	public:
		MultiThreadedTransform()
		{
			m_use_mask = false;
		}
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
