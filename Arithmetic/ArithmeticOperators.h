#pragma once

#include "../Core/Operator.h"

namespace NArray
{
	// supporting class for convolving an n-dimensional array with an n-dimensional kernel - tested
	// TODO could this be multi-threaded?
	template <class S, class V> class Convolve : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		const Index& m_index;
		V m_result;
		// member functions
	private:
		Convolve(const Convolve&);
		Convolve& operator =(const Convolve&);
	public:
		Convolve(const Index& index)
			: m_index(index)
		{
			Reset();
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& kernel)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			m_result += in.GetWindowedValue(m_index + index) * kernel.GetValue(index);
			return true;
		}
		V GetResult() const
		{
			return m_result;
		}
		void Reset()
		{
			GenericType gen_val(m_result);
			m_result = gen_val.GetAdditiveIdentityElement();
		}
	};

	// class for convolving an n-dimensional array with an n-dimensional kernel - tested
	template <class S, class V> class Convolution : public MultiThreadedOperator<S, V>
	{
		// member variables
	private:
		S& m_kernel;
		Mask m_mask;
		// member functions
	private:
		Convolution(const Convolution&);
		Convolution& operator =(const Convolution&);
	public:
		Convolution(S& kernel)
			: m_kernel(kernel)
		{
			m_mask.ReMask(m_kernel.GetOrder());
			m_mask.Initialise();
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			Convolve<S, V> convolve(index);
			m_kernel.ApplySingleThreadedOperator(m_mask, convolve, in, m_kernel);
			out.PutValue(index, convolve.GetResult());
			return true;
		}
		void Reset(S& kernel)
		{
			m_kernel = kernel;
		}
	};

	// class for offsetting the values in an n-dimensional array by a fixed amount - tested
	template <class S, class V> class Offset : public MultiThreadedOperator<S, V>
	{
		// member variables
	private:
		V m_offset;
		// member functions
	public:
		Offset()
		{
			Reset();
		}
		Offset(V offset)
		{
			Reset(offset);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value += m_offset;
			return true;
		}
		void Reset()
		{
			GenericType gen_val(m_offset);
			m_offset = gen_val.GetMultiplicativeIdentityElement();
		}
		void Reset(V offset)
		{
			m_offset = offset;
		}
	};

	// class for multiplying the values in an n-dimensional array together to generate a single value for the entire array - tested
	// this operator should not be multi-threaded because it is stateful
	// access to the state would need to be mutexed and this would result in poor performance (thread-contention)
	template <class S, class V> class Pi : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		V m_pi;
		// member functions
	public:
		Pi()
		{
			Reset();
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			m_pi *= value;
			return true;
		}
		V GetPi() const
		{
			return m_pi;
		}
		void Reset()
		{
			GenericType gen_val(m_pi);
			m_pi = (V) gen_val.GetMultiplicativeIdentityElement();
		}
	};

	// class for scaling the values in an n-dimensional array by a fixed amount - tested
	template <class S, class V> class Scale : public MultiThreadedOperator<S, V>
	{
		// member variables
	private:
		V m_scale;
		// member functions
	public:
		Scale(V scale)
		{
			Reset(scale);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value *= m_scale;
			return true;
		}
		void Reset(V scale)
		{
			m_scale = scale;
		}
	};

	// class for adding the values in an n-dimensional array together to generate a single value for the entire array - tested
	// this operator should not be multi-threaded because it is stateful
	// access to the state would need to be mutexed and this would result in poor performance (thread-contention)
	template <class S, class V> class Sigma : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		V m_sigma;
		// member functions
	public:
		Sigma()
		{
			Reset();
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			m_sigma += value;
			return true;
		}
		V GetSigma() const
		{
			return m_sigma;
		}
		void Reset()
		{
			GenericType gen_val(m_sigma);
			m_sigma = (V) gen_val.GetAdditiveIdentityElement();
		}
	};
}
