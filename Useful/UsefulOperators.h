#pragma once

#include <complex>
#include <random>
#include <time.h>

#include "../Core/NArrayDefinition.h"
#include "../GenericType/GenericType.h"

namespace NArray
{
	// class for copying the values in an n-dimensional array to their transpose - tested
	template <class S, class V> class CopyToTranspose : public MultiThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out.PutValue(index.Transpose(), value);
			return true;
		}
	};

	// class for calculating the absolute differences in row values between two columns in a matrix, writing them to the rows of a column in a second matrix - tested
	// TODO can this be multi-threaded?
	template <class S, class V> class DiffTwoColumns : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		int m_in_col1;
		int m_in_col2;
		int m_out_col;
		// member functions
	public:
		DiffTwoColumns(int in_col1, int in_col2, int out_col)
			: m_in_col1(in_col1), m_in_col2(in_col2), m_out_col(out_col)
		{
			// all initialisation done in initialiser list
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			Index in_index1(index);
			in_index1.PutIndexAtCoord(2, m_in_col1);
			Index in_index2(index);
			in_index2.PutIndexAtCoord(2, m_in_col2);
			Index out_index(index);
			out_index.PutIndexAtCoord(2, m_out_col);
			out.PutValue(out_index, abs(in.GetValue(in_index1) - in.GetValue(in_index2)));
			return true;
		}
	};

	// class for calculating the absolute differences in row values between all pairwise combinations of columns in a matrix, writing them to the rows of columns in a second matrix - tested
	// TODO can this be multi-threaded?
	template <class S1, class V1, class S2, class V2> class DiffAllColumns : public SingleThreadedTransform<S1, V1, S1, V1>
	{
		// member variables
	private:
		int m_out_col;
		const S2& m_pairwise_cols;
		// member functions
	private:
		DiffAllColumns(const DiffAllColumns&);
		DiffAllColumns& operator =(const DiffAllColumns&);
	public:
		DiffAllColumns(const S2& pairwise_cols)
			: m_pairwise_cols(pairwise_cols)
		{
			Reset();
		}
		virtual bool ApplyTransform(const Index& index, const V1& value, const S1& in, S1& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			if (m_pairwise_cols.GetValue(index))
			{
				S1& non_const_in = const_cast<S1&>(in);
				int in_col1 = index.GetIndexAtCoord(1) + 1;
				int in_col2 = index.GetIndexAtCoord(2) + 1;
				DiffTwoColumns<S1, V1> diff_two_columns(in_col1, in_col2, m_out_col++);
				Mask mask(c_matrix_order, Mask::use_all, 1);
				non_const_in.ApplySingleThreadedOperator<S1, V1>(mask, diff_two_columns, in, out);
				return true;
			}
			else
			{
				return false;
			}
		}
		void Reset()
		{
			m_out_col = 1;
		}
	};

	// class for getting the values in an n-dimensional array of booleans so that they can be compared logically - tested
	// this operator cannot be multi-threaded because it's return value is needed for further computation
	template <class S, class V> class GetValues : public SingleThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			return value;
		}
	};

	// class for determining whether the indices in an n-dimensional array are above the diagonal - tested
	// this operator cannot be multi-threaded because it's return value is needed for further computation
	template <class S, class V> class IsAboveDiag : public SingleThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			GreaterThan<int> greater_than;
			return NArraySupport::QueryIndex(index, greater_than);
		}
	};

	// class for determining whether the indices in an n-dimensional array are below the diagonal - tested
	// this operator cannot be multi-threaded because it's return value is needed for further computation
	template <class S, class V> class IsBelowDiag : public SingleThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			LessThan<int> less_than;
			return NArraySupport::QueryIndex(index, less_than);
		}
	};

	// class for determining whether the indices in an n-dimensional array are not on the diagonal - tested
	// this operator cannot be multi-threaded because it's return value is needed for further computation
	template <class S, class V> class IsNotOnDiag : public SingleThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			NotEqual<int> not_equal;
			return NArraySupport::QueryIndex(index, not_equal);
		}
	};

	// class for determining whether the indices in an n-dimensional array are on the diagonal - tested
	// this operator cannot be multi-threaded because it's return value is needed for further computation
	template <class S, class V> class IsOnDiag : public SingleThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			Equal<int> equal;
			return NArraySupport::QueryIndex(index, equal);
		}
	};

	// class for setting the values in a matrix of std::complex<double> so as to create the Complex Plane (ramped along the real axis, ramped along the imaginary axis) - tested
	template <class S, class V> class SetToComplexPlane : public MultiThreadedOperator<S, V>
	{
		// member variables
	private:
		std::complex<double> m_start_val;
		std::complex<double> m_step_val;
		// member functions
	public:
		SetToComplexPlane()
		{
			Reset();
		}
		SetToComplexPlane(std::complex<double> start_val, std::complex<double> step_val)
		{
			Reset(start_val, step_val);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const NArrayType<std::complex<double>>::Matrix& in, NArrayType<std::complex<double>>::Matrix& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out.PutValue(index, CalcRamp(index));
			return true;
		}
		std::complex<double> CalcRamp(const Index& index)
		{
			double start_val_real = m_start_val.real();
			double start_val_imag = m_start_val.imag();
			double step_val_real = m_step_val.real();
			double step_val_imag = m_step_val.imag();
			int pos_real = index.GetIndexAtCoord(2);
			int pos_imag = index.GetIndexAtCoord(1);
			double val_real = start_val_real + (pos_real * step_val_real);
			double val_imag = start_val_imag + (pos_imag * step_val_imag);
			std::complex<double> temp(val_real, val_imag);
			return temp;
		}
		void Reset()
		{
			GenericType gen_val(m_start_val);
			m_start_val = gen_val.GetAdditiveIdentityElement();
			m_step_val = gen_val.GetMultiplicativeIdentityElement();
		}
		void Reset(std::complex<double> start_val, std::complex<double> step_val)
		{
			m_start_val = start_val;
			m_step_val = step_val;
		}
	};

	// class for setting the values in an n-dimensional array to one (their multiplicative identity element) - tested
	template <class S, class V> class SetToOne : public MultiThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			GenericType gen_val(value);
			value = gen_val.GetMultiplicativeIdentityElement();
			return true;
		}
	};

	// class for setting the values in an n-dimensional array to be ramped (monotonically increasing or decreasing) - tested
	// this operator should not be multi-threaded because it is stateful
	// access to the state would need to be mutexed and this would result in poor performance (thread-contention)
	template <class S, class V> class SetToRamped : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		V m_current_val;
		V m_start_val;
		V m_step_val;
		// member functions
	public:
		SetToRamped()
		{
			Reset();
		}
		SetToRamped(V start_val, V step_val)
		{
			Reset(start_val, step_val);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value = CalcRamp();
			return true;
		}
		V CalcRamp()
		{
			V temp = m_current_val;
			m_current_val += m_step_val;
			return temp;
		}
		void Reset()
		{
			GenericType gen_val(m_current_val);
			m_start_val = (V) gen_val.GetAdditiveIdentityElement();
			m_step_val = (V) gen_val.GetMultiplicativeIdentityElement();
			Restart();
		}
		void Reset(V start_val, V step_val)
		{
			m_start_val = start_val;
			m_step_val = step_val;
			Restart();
		}
		void Restart()
		{
			m_current_val = m_start_val;
		}
	};

	// class for setting the values in an n-dimensional array to randomly generated values using the rand() generator - tested
	// this operator should not be multi-threaded because rand() is not re-entrant
	template <class S, class V> class SetToRandom : public SingleThreadedOperator<S, V>
	{
		// member variables
	protected:
		V m_max;
		V m_min;
		// member fuctions
	public:
		SetToRandom()
		{
			Reset();
		}
		SetToRandom(V min, V max)
		{
			Reset(min, max);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value = CalcRandNum();
			return true;
		}
		virtual V CalcRandNum() const
		{
			double rnd_num = (double) rand() / (double) RAND_MAX;
			return (V) (m_min + (rnd_num * (m_max - m_min)));
		}
		void Reset()
		{
			GenericType gen_val(m_max);
			m_min = gen_val.GetAdditiveIdentityElement();
			m_max = gen_val.GetMultiplicativeIdentityElement();
			Restart();
		}
		void Reset(V min, V max)
		{
			m_min = min;
			m_max = max;
			Restart();
		}
		virtual void Restart() const
		{
			// srand((unsigned int) time(nullptr)); // TODO have a flag to determine whether repeatable sequences are desired or not
		}
	};

	// partial specialisation of SetToRandom for booleans - tested
	// this operator should not be multi-threaded because rand() is not re-entrant
	template <class S> class SetToRandom<S, bool> : public SingleThreadedOperator<S, bool>
	{
		// member fuctions
	public:
		SetToRandom()
		{
			Restart();
		}
		SetToRandom(bool b1, bool b2) // need this constructor to satisfy compiler
		{
			b1; // do this to prevent compiler warning 'unreferenced formal parameter'
			b2; // do this to prevent compiler warning 'unreferenced formal parameter'
			Restart();
		}
		virtual bool ApplyOperator(const Index& index, bool& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value = CalcRandNum();
			return true;
		}
		virtual bool CalcRandNum() const
		{
			double rnd_num = (double) rand() / (double) RAND_MAX;
			if (rnd_num < 0.5)
				return false;
			else
				return true;
		}
		virtual void Restart() const
		{
			// srand((unsigned int) time(nullptr)); // TODO have a flag to determine whether repeatable sequences are desired or not
		}
	};

	// partial specialisation of SetToRandom for std::complex<double> - tested
	// this operator should not be multi-threaded because rand() is not re-entrant
	template <class S> class SetToRandom<S, std::complex<double>> : public SingleThreadedOperator<S, std::complex<double>>
	{
		// member variables
	protected:
		double m_imag_max;
		double m_imag_min;
		double m_real_max;
		double m_real_min;
		// member fuctions
	public:
		SetToRandom()
		{
			Reset();
		}
		SetToRandom(std::complex<double> min, std::complex<double> max)
		{
			Reset(min, max);
		}
		virtual bool ApplyOperator(const Index& index, std::complex<double>& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value = CalcRandNum();
			return true;
		}
		virtual std::complex<double> CalcRandNum() const
		{
			double rnd_num;
			rnd_num = (double) rand() / (double) RAND_MAX;
			double real_rnd_num = m_real_min + (rnd_num * (m_real_max - m_real_min));
			rnd_num = (double) rand() / (double) RAND_MAX;
			double imag_rnd_num = m_imag_min + (rnd_num * (m_imag_max - m_imag_min));
			return std::complex<double>(real_rnd_num, imag_rnd_num);
		}
		void Reset()
		{
			m_real_min = 0.0;
			m_real_max = 1.0;
			m_imag_min = 0.0;
			m_imag_max = 1.0;
			Restart();
		}
		void Reset(std::complex<double> min, std::complex<double> max)
		{
			m_real_min = min.real();
			m_real_max = max.real();
			m_imag_min = min.imag();
			m_imag_max = max.imag();
			Restart();
		}
		virtual void Restart() const
		{
			// srand((unsigned int) time(nullptr)); // TODO have a flag to determine whether repeatable sequences are desired or not
		}
	};

	// class for setting the values in an n-dimensional array to the same value - tested
	// this operator should not be multi-threaded because it is stateful
	// access to the state would need to be mutexed and this would result in poor performance (thread-contention)
	template <class S, class V> class SetToValue : public SingleThreadedOperator<S, V>
	{
		// member variables
	private:
		V m_value;
		// member functions
	public:
		SetToValue()
		{
			Reset();
		}
		SetToValue(V value)
		{
			Reset(value);
		}
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			value = m_value;
			return true;
		}
		void Reset()
		{
			GenericType gen_val(m_value);
			m_value = gen_val.GetDefaultValue();
		}
		void Reset(V value)
		{
			m_value = value;
		}
	};

	// class for setting the values in an n-dimensional array to zero (their additive identity element) - tested
	template <class S, class V> class SetToZero : public MultiThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			index; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out; // do this to prevent compiler warning 'unreferenced formal parameter'
			GenericType gen_val(value);
			value = gen_val.GetAdditiveIdentityElement();
			return true;
		}
	};

	// class for swapping the values in an n-dimensional array with their transpose - tested
	template <class S, class V> class SwapWithTranspose : public MultiThreadedOperator<S, V>
	{
		// member functions
	public:
		virtual bool ApplyOperator(const Index& index, V& value, const S& in, S& out)
		{
			value; // do this to prevent compiler warning 'unreferenced formal parameter'
			in; // do this to prevent compiler warning 'unreferenced formal parameter'
			out.SwapValue(index, index.Transpose());
			return true;
		}
	};
}
